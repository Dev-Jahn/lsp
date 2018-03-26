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
void preprocess(const char *pathname, Pair macro[MAX_MACRO])
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
	//include 처리
	incl(fd2);
	//주석처리
	do {
		offsets = regfind(fd2, "^#.*");
		delLine(fd2);
	} while (offsets.found);
	lseek(fd2,0,SEEK_SET);	
	//매크로
	char *essential =	"^\\w+\\s*=\\s*\"?\\w+\"?";
	char *optional =	"^\\w+\\s*\\?=\\s*\"?\\w+\"?";
	char *var =			"\\$[\\(\\{]\\w+[\\)\\}]";
	char *inner1 =		"\\$\\*";
	char *inner2 =		"\\$@";
	char *target =		"\\w+\\s*+:(\\s*[\\w\\.]+)+";
	char *cmd =			"^\\t[^\\s]+.*";
	char line[LINE_SIZE], *key, *value, *saveptr, *delim = " \t";
	List macroList;
	initList(&macroList);
	do {
		offsets = regfind(fd2,essential);
		readLine(fd2,line);
		key = strtok_r(line, delim, &saveptr);
		strtok_r(NULL, delim, &saveptr);
		value = strtok_r(NULL, delim, &saveptr);
		Pair *m = (Pair*)malloc(sizeof(Pair)); 
		m->key = key;
		m->value = value;
		addNode(&macroList, m);
	} while(offsets.found);
	lseek(fd2, 0,SEEK_SET);
	do {
		char symbol[WORD_SIZE] = "$(";
		strcat(symbol, ((Pair*)macroList.cur->item)->key);
		strcat(symbol, ")");




		

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
