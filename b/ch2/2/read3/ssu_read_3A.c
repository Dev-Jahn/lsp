#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char c;
	int fd;
	//읽기로 파일열기
	if ((fd  = open("ssu_test.txt", O_RDONLY)) < 0)
	{
		fprintf(stderr, "open error for %s\n", "ssu_test.txt");
		exit(1);
	}
	//파일 끝까지 반복
	while(1)
	{
		//1바이트 읽기
		if (read(fd, &c, 1) > 0)
			//출력
			putchar(c);
		//파일끝에서 0이 리턴되면 탈출
		else
			break;
	}

	exit(0);
}
