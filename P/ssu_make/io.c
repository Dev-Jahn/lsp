#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include "struct.h"
#include "util.h"
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
	fprintf(stderr,"ssu_make: no eol at the end of the file\n");
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
	for (i=0; i<cnt;i++)
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

/* ---------------------------------*/
/**
 * @brief 파일의 현재 오프셋 뒤에 pathname 파일 전체를 삽입 
 *
 * @param fd_dst 붙여넣을 목적지 file descriptor
 * @param pathname 붙여넣을 내용 파일의 경로
 *
 * @return 붙여넣은 총 길이, 원본 파일 오프셋은 붙인내용 뒤로 이동
 */
/* ---------------------------------*/
ssize_t fconcat(int fd_dst, const char *pathname)
{
	
	off_t seek_org = lseek(fd_dst, 0, SEEK_CUR);
	off_t seek_end = lseek(fd_dst, 0, SEEK_END);
	off_t residue = seek_end - seek_org;
	lseek(fd_dst, seek_org, SEEK_SET);

	char buf[BUFFER_SIZE];
	int fd_src;
	ssize_t length, total = 0;
	if ((fd_src = open(pathname, O_RDONLY))<0)
	{
		fprintf(stderr, "ssu_make: open error %d for %s in fconcat\n", fd_src, pathname);
		exit(1);
	}
	while ((length = read(fd_src, buf, BUFFER_SIZE))>0)
		total+=length;

	//붙여넣어질 영역의 원본 파일 밀어내기
	for(int i = 0;i<((int)residue)/BUFFER_SIZE;i++)
	{
		off_t cursor = seek_end - (i+1) * ((off_t)BUFFER_SIZE);
		lseek(fd_dst, cursor, SEEK_SET);
		length = read(fd_dst, buf, BUFFER_SIZE);
		lseek(fd_dst, cursor + (off_t)total, SEEK_SET);
		write(fd_dst, buf, length);
	}
	lseek(fd_dst, seek_org, SEEK_SET);
	length = read(fd_dst, buf, (int)residue%BUFFER_SIZE);
	lseek(fd_dst, seek_org + (off_t)total, SEEK_SET);
	write(fd_dst, buf, length);

	//src파일 붙여넣기
	lseek(fd_dst, seek_org, SEEK_SET);	
	lseek(fd_src, 0, SEEK_SET);
	while ((length = read(fd_src, buf, BUFFER_SIZE))>0)
		write(fd_dst, buf, length);
	close(fd_src);
	return total;
}

/* ---------------------------------*/
/**
 * @brief 현재 오프셋 이후의 모든 dst문자열을 src로 대체
 *
 * @param fd O_RDWR로 오픈된 파일디스크립터
 * @param pattern 치환될 대상 문자열의 정규표현식 패턴
 * @param src 삽입할 문자열
 *
 * @return 함수실행전후 파일크기 차(byte)를 리턴
 */
/* ---------------------------------*/
ssize_t freplace(int fd, const char *pattern, const char *src)
{
	Off_Pair offsets;
	off_t seek_org = lseek(fd, 0, SEEK_CUR);
	off_t residue;
	ssize_t length, diff, diffsum = 0;
	char buf[BUFFER_SIZE];
	lseek(fd, 0, seek_org);
	do {
		offsets = regfind(fd, pattern);
		if(offsets.found == 0 )
			break;
		lseek(fd, offsets.eo, SEEK_SET);
		off_t seek_end = lseek(fd, 0, SEEK_END);
		residue = seek_end - offsets.eo;	//이동할 블록크기
		diff = strlen(src) + offsets.so - offsets.eo;	//이동할 거리
		diffsum += diff;
		if (diff != 0)
		{
			for(int i = 0;i<((int)residue)/BUFFER_SIZE;i++)
			{
				off_t cursor = seek_end - (i+1) * ((off_t)BUFFER_SIZE);
				lseek(fd, cursor, SEEK_SET);
				length = read(fd, buf, BUFFER_SIZE);
				lseek(fd, cursor + (off_t)diff, SEEK_SET);
				write(fd, buf, length);

			}
			lseek(fd, offsets.eo, SEEK_SET);
			length = read(fd, buf, (int)residue%BUFFER_SIZE);
			lseek(fd, offsets.eo + (off_t)diff, SEEK_SET);
			write(fd, buf, length);
			if (diff<0)
				ftruncate(fd, seek_end+diff);
		}
		lseek(fd, offsets.so, SEEK_SET);
		write(fd, src, strlen(src));
	} while (offsets.found);

	lseek(fd, 0, SEEK_SET);
	return diffsum;
}

void erase(int filedes, off_t so, off_t eo)
{
	char *buf = (char*)malloc(eo-so+1);
	buf[eo-so]=0;
	memset(buf,' ',eo-so);
	lseek(filedes, so, SEEK_SET);
	write(filedes, buf, eo-so);
	free(buf);
}
