#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	char character;
	int fd;
	int line_count = 0;

	//읽기로 파일열기
	if ((fd = open("1", O_RDONLY)) <0)
	{
		fprintf(stderr, "open error for %s\n", "ssu_test.txt");
		exit(1);
	}
	//파일끝까지 반복
	 while(1)
	 {
		//1바이트 읽기
		if (read(fd, &character, 1) > 0)
		{
			//개행문자이면
			if (character == '\n')
				//카운트증가
				line_count++;
		}
		//파일끝이면 탈출
		else
			break;
	 }
	 //총 라인수 출력
	 printf("Total line : %d\n", line_count);
	 exit(0);
}
