#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	//creat할 파일명
	char *fname = "ssu_test.txt";
	//할당할 파일디스크립터
	int fd;
	//파일 creat
	if ((fd = creat(fname, 0666))<0)
	{
		printf("creat error for %s\n", fname);
		exit(1);
	}
	//표준출력으로 문자열 출력
	printf("First printf is on the screen.\n");
	//표준출력을 close하고 1번 디스크립터에 fd복제
	dup2(fd, 1);
	//1번 디스크립터(생성한 파일)로 문자열 출력
	printf("Second printf is in this file.\n");
	exit(0);
}
