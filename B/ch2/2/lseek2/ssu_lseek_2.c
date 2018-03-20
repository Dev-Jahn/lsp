#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
//creat시 적용할 파일모드 0644
#define CREAT_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

//사용할 문자열 버퍼
char buf1[] = "1234567890";
char buf2[] = "ABCDEFGHIJ";

int main(void)
{
	//생성할 파일명
	char *fname = "ssu_hole.txt";
	int fd;

	//파일생성, 에러처리
	if ((fd = creat(fname, CREAT_MODE)) < 0)
	{
		fprintf(stderr, "creat error for %s\n", fname);
		exit(1);
	}
	//파일오프셋 시작부터 buf1 내용 12바이트 쓰기	
	if (write(fd, buf1, 12) != 12)
	{
		fprintf(stderr, "buf1 write error\n");
		exit(1);
	}

	//파일 오프셋 15000바이트 이동
	if (lseek(fd, 15000, SEEK_SET) < 0)
	{
		fprintf(stderr, "lseek error\n");
		exit(1);
	}

	//buf2내용 12바이트 쓰기
	if (write(fd, buf2, 12) != 12)
	{
		fprintf(stderr, "buf2 write error\n");
		exit(1);
	}

	exit(0);
}
