#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char buf[BUFFER_SIZE];
	char *fname = "ssu_test.txt";
	int fd;
	int length;
	//파일 읽기로 open
	if ((fd = open(fname, O_RDONLY, 0644))<0)
	{
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}
	//표준출력을 4번 디스크립터로 복제
	if (dup2(1,4)!=4)
	{
		fprintf(stderr, "dup2 call failed\n");
		exit(1);
	}
	//파일 끝까지 반복
	while(1)
	{
		//버퍼단위로 읽어서
		length = read(fd, buf, BUFFER_SIZE);
		if (length <=0)
			break;
		//표준출력을 복제한 4번 디스크립터로 출력
		write(4, buf, length);
	}
	exit(0);
}
