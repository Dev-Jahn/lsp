#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define S_MODE 0644
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	char buf[BUFFER_SIZE];
	int fd1, fd2;
	int length;
	//인자의 형태가 잘못되었을때	
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s filein fileout\n", argv[0]);
		exit(1);
	}
	//첫번째인자의 파일을 읽기로 열어 fd1에 할당	
	if ((fd1 = open(argv[1], O_RDONLY))<0)
	{
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}
	//두번째인자의 파일을 열어 fd2에 할당
	//쓰기전용, 없으면 생성, 있으면 삭제, 모드 644
	if ((fd2 = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_MODE))<0)
	{
		fprintf(stderr, "open error for %s\n", argv[2]);
		exit(1);
	}

	//버퍼 크기단위로 파일을 읽어 fd2에 열린 파일로 쓰기
	while ((length = read(fd1, buf, BUFFER_SIZE)) > 0)
	{
		write(fd2, buf, length);
	}

	exit(0);
}
