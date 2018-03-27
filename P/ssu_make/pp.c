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

/* ---------------------------------*/
/**
 * @brief 전처리기 
 * 1.include 위치에 파일 삽입
 * 2.주석제거
 * 3.파일내 매크로처리
 * 4.명령행 매크로처리
 * 5.줄바꿈 처리
 * 6.내부 매크로처리
 *
 * @param pathname 처리할 Makefile의 경로
 * @param macro[MAX_MACRO] Makefile에 정의할 명령행 매크로
 */
/* ---------------------------------*/
void preprocess(const char *pathname, Pair *cmd_macro[MAX_MACRO])
{
	//처리가 완료된 파일이 저장될 path : pathname.tmp
	char output[WORD_SIZE];
	strcat(strcpy(output, pathname), ".tmp");
	int fd1, fd2, length;
	char buf[BUFFER_SIZE];
	Off_Pair offsets;

	//전처리후 상태를 저장할 임시파일 생성, 원본 복사
	if ((fd1 = open(pathname, O_RDONLY))<0)
	{
		fprintf(stderr, "open error for %s\n", pathname);
		exit(1);
	}
	if ((fd2 = open(output, O_RDWR|O_CREAT|O_TRUNC, S_MODE))<0)
	{	
		fprintf(stderr, "open error for %s\n", pathname);
		exit(1);
	}
	while ((length = read(fd1, buf, BUFFER_SIZE))>0)
		write(fd2, buf, length);
	close(fd1);
	lseek(fd2, 0, SEEK_SET);
	//줄바꿈 처리
	freplace(fd2, "\\\\\n", "");
	//include 처리
	incl(fd2);
/*
 *	ERROR
 *	행단위 문법검사
 *
 */
	//주석처리
	freplace(fd2, "^#.*\n", "");

	lseek(fd2,0,SEEK_SET);	
	//매크로
	char line[LINE_SIZE], *key, *value, *saveptr, *delim = " \t?=";
	char save[LINE_SIZE];
	List macroList;
	initList(&macroList);
	do {
		offsets = regfind(fd2, pat_macro);
		if (offsets.found == 0)
			break;
		readLine(fd2,line);
		memcpy(save,line,LINE_SIZE);
		key = strtok_r(line, delim, &saveptr);
		if (compare(pat_quot, save) == 0)
		{
			delim = "?=";
			value = strtok_r(NULL, delim, &saveptr);
			delim = " \t?=";
		}
		else
			value = strtok_r(NULL, delim, &saveptr);
		Pair *m = newPair(key, value);
		//매크로 키가 이미 추가되어 있는지  리스트검사
		Node *exist = searchList(&macroList, m, compKey);
		if (exist!=NULL) //존재
		{
			printf("%s:%s exists\n",m->key,m->value);
			//옵셔널 매크로이면 생략
			if (compare(pat_optional, save) == 0)
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
	for (int i=0; cmd_macro!=NULL && cmd_macro[i]!=NULL; i++)
	{
		Node *exist = searchList(&macroList, cmd_macro[i], compKey);
		if (exist!=NULL)
			exist->item = cmd_macro[i];
		else
			addNode(&macroList, cmd_macro[i]);
	}

	lseek(fd2, 0,SEEK_SET);
	//매크로 키로 매크로심볼을 만들어 검색
	//일치하는 모든 심볼을 치환
	macroList.cur = macroList.head;
	do {
		char symbol[WORD_SIZE]; 
		strcpy(symbol, pat_var_l);
		strcat(symbol, ((Pair*)macroList.cur->item)->key);
		strcat(symbol, pat_var_r);
		printf("%s\n",symbol);
		freplace(fd2, symbol, ((Pair*)macroList.cur->item)->value);
		macroList.cur = macroList.cur->next;
	} while (macroList.cur != NULL);
/*
 * ERROR
 * 치환 후 남은 심볼이 있을때 에러처리
 */
	freplace(fd2, pat_macro, "");
	freplace(fd2, pat_blank, "");
	close(fd2);
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
