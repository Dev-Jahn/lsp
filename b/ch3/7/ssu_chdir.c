#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	//작업디렉토리 /etc로 변경
	if (chdir("/etc")<0)
	{
		//실패시 에러
		fprintf(stderr, "chdir error\n");
		exit(1);
	}
	//성공시 출력
	printf("chdir to /etc succeeded.\n");
	exit(0);
}

