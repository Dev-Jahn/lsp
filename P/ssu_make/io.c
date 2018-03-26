#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"

/* ---------------------------------*/
/**
 * @brief 파일을 현재 오프셋에서 한줄 읽어 버퍼에 저장하는 함수 
 * 읽은후 오프셋은 다음줄 시작
 * 개행문자는 포함하지 않음
 *
 * @param filedes 읽기 가능한 상태로 오픈된 파일디스크립터
 * @param buf_ 읽은 문자열을 저장할 버퍼
 *
 * @return 읽은 문자열의 길이(개행제외).파일 끝이면 EOF.
 */
/* ---------------------------------*/
ssize_t readLine(int filedes, void *buf_)
{
	char *buf = (char*)buf_;
	ssize_t i,  cnt = read(filedes, buf, LINE_SIZE);
	if (cnt == 0)
		return EOF;
	buf[cnt] = '\0';
	for(i=0; buf[i]; i++)
		if (buf[i] == '\n')
		{
			buf[i] = '\0';
			lseek(filedes, (off_t)(-cnt+i+1), SEEK_CUR);
			return i;
		}
	fprintf(stderr,"no eol at the end of the file\n");
	exit(1);
}

/* ---------------------------------*/
/**
 * @brief 파일의 현재 오프셋부터 한줄을 공백으로 치환하는 함수
 * 실행후 오프셋은 다음줄의 시작
 *
 * @param filedes 쓰기 가능한 상태로 오픈된 파일디스크립터
 *
 * @return 공백으로 대체한 총 문자 수.파일끝까지 개행이 없으면 -1
 */
/* ---------------------------------*/
ssize_t delLine(int filedes)
{
	char buf[LINE_SIZE];
	ssize_t i, cnt = read(filedes, buf,LINE_SIZE);
	buf[cnt] = 0;
	lseek(filedes, (off_t)(-cnt), SEEK_CUR);
	for (i=0; buf[i];i++)
	{
		if (buf[i] == '\n')
		{
			lseek(filedes, 1, SEEK_CUR);
			return i;
		}
		else
			write(filedes, " ", 1);
	}
	return (ssize_t)(-1);
}
