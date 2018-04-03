#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <regex.h>
#include "struct.h"
#include "util.h"
#include "io.h"
#include "constants.h"
#include "pp.h"
#include "patterns.h"
#include "errno.h"
#include "main.h"

List macroList;		//전역매크로리스트
/* ---------------------------------*/
/**
 * @brief 전처리기 
 * 1.행단위 문법검사
 * 2.줄바꿈 처리
 * 3.include 위치에 파일 삽입
 * 4.주석제거
 * 5.파일내 매크로 리스트에 추가
 * 6.명령행 매크로 리스트에 추가
 * 7.리스트내의 매크로에 대응하는 모든 매크로심볼 치환
 * 8.내부 매크로처리
 * 9.매크로선언, 빈줄 삭제
 *
 * @param pathname 처리할 Makefile의 경로
 * @param from_cmd 명령행에서 입력받은 매크로 목록
 * 
 * @return 전처리완료후 열린상태의 파일디스크립터(O_RDWR)
 */
/* ---------------------------------*/
int preprocess(const char *pathname, List *from_cmd)
{
	//처리가 완료된 파일이 저장될 path : pathname.pp
	char output[WORD_SIZE];
	strcat(strcpy(output, pathname), ".pp");
	int fd1, fd2, length;
	char buf[BUFFER_SIZE];
	Off_Pair offsets;
	struct stat statbuf;
	stat(pathname,&statbuf);
	//전처리후 상태를 저장할 임시파일 생성, 원본 복사
	if ((fd1 = open(pathname, O_RDONLY))<0)
	{
		fprintf(stderr, "ssu_make: There's no file \"%s\".\n",pathname);
		exit(1);
	}
	if (!S_ISREG(statbuf.st_mode))
	{
		fprintf(stderr, "ssu_make: \"%s\" is not a regular file.\n",pathname);
		exit(1);
	}
	if ((fd2 = open(output, O_RDWR|O_CREAT|O_TRUNC, S_MODE))<0)
	{	
		fprintf(stderr, "ssu_make: open error for %s\n", pathname);
		exit(1);
	}
	while ((length = read(fd1, buf, BUFFER_SIZE))>0)
		write(fd2, buf, length);
	close(fd1);
	lseek(fd2, 0, SEEK_SET);
	//행단위 문법검사
	syntax(pathname);
	//줄바꿈 처리
	freplace(fd2, pat_newline, "");
	//include 처리
	incl(fd2);
	//주석처리
	freplace(fd2, pat_comment, "");
	lseek(fd2,0,SEEK_SET);	
	//매크로
	char line[LINE_SIZE], *key, *value;
	initList(&macroList);
	do {
		offsets = regfind(fd2, pat_macro);
		if (offsets.found == 0)
			break;
		readLine(fd2,line);
		key = trim("\\w+", line);
		char *tmp = trim("=\\s*([^ 	]+\\s*)+$",line);
		value = trim("([^ 	=]+\\s*)+$",tmp);
		free(tmp);
		Pair *m = newPair(key, value);
		//매크로 키가 이미 추가되어 있는지  리스트검사
		Node *exist = searchList(&macroList, m, compKey);
		if (exist!=NULL) //존재
		{
			//옵셔널 매크로이면 생략
			if (compare(pat_optional, line) == 0)
				continue;
			//일반 매크로이면 재정의
			else
				exist->item = m;
		}
		//리스트에 없으면 새로 추가
		else
			addNode(&macroList, m);
	} while(offsets.found);
	//명령행입력 매크로 리스트에 추가	
	if (from_cmd != NULL)
		for (from_cmd->cur = from_cmd->head;
			from_cmd->cur != NULL;
			from_cmd->cur = from_cmd->cur->next)
		{
			Node *exist = searchList(&macroList, (Pair*)from_cmd->cur->item, compKey);
			if (exist!=NULL)
				exist->item = (Pair*)from_cmd->cur->item;
			else
				addNode(&macroList, (Pair*)from_cmd->cur->item);
		}

	lseek(fd2, 0,SEEK_SET);
	//매크로 키로 매크로심볼을 만들어 검색
	//일치하는 모든 심볼을 치환
	macroList.cur = macroList.head;
	while (macroList.cur != NULL)
	{
		char symbol[WORD_SIZE]; 
		strcpy(symbol, pat_var_l);
		strcat(symbol, ((Pair*)macroList.cur->item)->key);
		strcat(symbol, pat_var_r);
		freplace(fd2, symbol, ((Pair*)macroList.cur->item)->value);
		macroList.cur = macroList.cur->next;
	}
/*
 * ERROR
 * 치환 후 남은 심볼이 있을때 에러처리
 */
	freplace(fd2, pat_macro, "");
	freplace(fd2, pat_blank_lf, "");
	inner(fd2);
	return fd2;
}

/* ---------------------------------*/
/**
 * @brief 파일에서 include문을 찾아 파일을 삽입하고 include문 삭제
 * 수행후 오프셋 SEEK_SET으로
 *
 * @param fd O_RDWR로 열린 파일디스크립터
 *
 * @return include로 삽입된 파일의 총 갯수
 */
/* ---------------------------------*/
int incl(int fd)
{
	char *saveptr, *tmp, *delim = " \t";
	int included = 0;
	char line[LINE_SIZE];
	Stack tokstk;
	initStack(&tokstk);

	Off_Pair offsets;
	lseek(fd, 0, SEEK_SET);
	do
	{
		offsets = regfind(fd, "include");	
		if (offsets.found)
		{
			//한줄 읽어온후 파일에서 해당줄 삭제
			readLine(fd, line);
			//include 키워드 삭제
			strncpy(line, "       ", 7);
			lseek(fd, offsets.so, SEEK_SET);
			delLine(fd);
			//읽은 줄 토큰으로 분할해 스택에 push
			char *tok = strtok_r(line, delim, &saveptr);
			if (tok != NULL)
				push(&tokstk, tok);
			while((tok = strtok_r(NULL, delim, &saveptr)) != NULL)
				push(&tokstk, tok);
			while(tokstk.size)
			{
				tmp = (char*)pop(&tokstk);
				ssize_t length = fconcat(fd, tmp);
				lseek(fd, (off_t)(-length), SEEK_CUR);
				included++;
			}
		}
	} while(offsets.found);
	lseek(fd, 0, SEEK_SET);
	return included;
}

/* ---------------------------------*/
/**
 * @brief 행단위로 파일을 읽어 Makefile의 문법에 적합한지 검사
 * 허용되는 형태
 * 매크로선언		(A=B 또는 A?=B)
 * include 문		(include path1 path2 path3 ...)
 * target 선언		(target : dep1 dep2 dep3 ...)
 * 커맨드			(	Any command here) 
 * 빈줄				(				)
 * 주석				(#This is comment)
 *
 * 줄바꿈시 줄바꾼 행을 전부 병합후 재검사
 *
 * @param pathname 처리할 파일의 경로명
 */
/* ---------------------------------*/
void syntax(const char *pathname)
{
	char tmp[WORD_SIZE];
	char line[LINE_SIZE];
	char buf[BUFFER_SIZE];
	int linecnt = 1, checksum, bytes, len;
	strcat(strcpy(tmp, pathname), ".syntax");
	int fd,src;
	if ((src = open(pathname, O_RDONLY))<0)
	{
		fprintf(stderr, "ssu_make: Can't open %s\n", pathname);
		exit(1);
	}
	if ((fd = open(tmp, O_RDWR|O_CREAT|O_TRUNC, S_MODE))<0)
	{	
		fprintf(stderr, "ssu_make: open error for %s\n", pathname);
		exit(1);
	}
	while ((len = read(src, buf, BUFFER_SIZE))>0)
		write(fd, buf, len);
	close(src);
	lseek(fd, 0, SEEK_SET);
	while ((bytes = readLine(fd, line)) != EOF)
	{
		checksum = 1;
		if (compare("^.*\\\\[ 	]*$",line) == 0)
		{
			lseek(fd,-bytes-1, SEEK_CUR);
			char *new = sreplace(line, "\\\\"," ");
			write(fd, new, bytes);
			write(fd, " ",1);
			lseek(fd,-bytes-1, SEEK_CUR);
			continue;
		}
		checksum *= compare(pat_macro, line);
		checksum *= compare(pat_include, line);
		checksum *= compare(pat_target, line);
		checksum *= compare(pat_cmd, line);
		checksum *= compare("", line);
		checksum *= compare(pat_blank, line);
		checksum *= compare(pat_comment, line);
		if (checksum != 0)
		{
			printf("ssu_make: Unknown syntax at line <%d> of <%s>.\n",linecnt, pathname);
			printf(">%s\n",line);
			break;
		}
		linecnt++;
	}
	close(fd);
	if (remove(tmp)!=0)
		fprintf(stderr, "ssu_make: Can't remove %s in syntax()\n",tmp);
	if (checksum == 0)
	{
		if (ON_D(flag))
			printf("ssu_make: Checked %d lines with no syntax error.\n",--linecnt);
	}
	else
	{
		exit(1);
	}
}

void inner(int fd)
{
	char line[LINE_SIZE],buf[4096];
	char *name;
	char *name_noex;
	char *dep;
	int cnt = 0;
	lseek(fd, 0, SEEK_SET);
	while ((cnt = readLine(fd, line)) != EOF)
	{
		if (compare(pat_target, line) == 0)
		{
			dep = trim(pat_dep,line);
			name = trim("^(\\w|\\.)+", line);
			name_noex = trim("^\\w+", name);
		}
		else if (compare(pat_cmd, line) == 0)
		{
			char *s1 = sreplace(line, pat_inner, name);
			char *s2 = sreplace(s1, pat_inner_noex, name_noex);
			char *s3 = sreplace(s2, pat_inner_dep, dep);
			if (s3 == NULL)
				continue;
			/*printf("%s\n",s3);*/
			off_t so = lseek(fd, 0, SEEK_CUR);
			ssize_t size = read(fd,buf,4096);
			lseek(fd, so + (strlen(s3) - cnt),SEEK_SET);
			write(fd,buf,size);
			lseek(fd, so - cnt - 1, SEEK_SET);
			write(fd,s3, strlen(s3));
			write(fd,"\n",1);
			free(s1);
			free(s2);
			free(s3);
		}
	}
	lseek(fd, 0, SEEK_SET);
}
