#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <regex.h>
#include "struct.h"

#define S_MODE 0644
#define BUFFER_SIZE 1024
/*
 * 전처리기
 * 1.include 위치에 파일내용 삽입
 * 2.주석제거
 * 3.파일내의 매크로 처리
 * - 인자로 입력받은 매크로 덮어쓰기
 */
void preprocess(const char *pathname, Pair macro[MAX_MACRO])
{
	//처리가 완료된 파일이 저장될 path : pathname.tmp
	char *output = (char*)malloc(sizeof(char)*(strlen(pathname)+strlen(".tmp")+1));
	strcat(strcpy(output, pathname), ".tmp");

	int fd1, fd2;
	char buf[BUFFER_SIZE];
	int length;

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

	/*
	 * include
	 */
	regex_t
}
