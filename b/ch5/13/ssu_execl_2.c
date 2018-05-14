#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	//첫번째 자식프로세스
	if (fork()==0)
	{
		//echo로 인자 전달해 문자열 출력
		execl("/bin/echo", "echo", "this is", "message one", (char *)0);
		//정상 실행시 이후는 실행하지 않음
		fprintf(stderr, "exec error\n");
		exit(1);
	}
	//두번째 자식프로세스
	if (fork()==0)
	{
		//echo로 인자 전달해 문자열 출력
		execl("/bin/echo", "echo", "this is", "message two", (char *)0);
		//정상 실행시 이후는 실행하지 않음
		fprintf(stderr, "exec error\n");
		exit(1);
	}
	//세번째 자식프로세스
	if (fork()==0)
	{
		//echo로 인자 전달해 문자열 출력
		execl("/bin/echo", "echo", "this is", "message three", (char *)0);
		//정상 실행시 이후는 실행하지 않음
		fprintf(stderr, "exec error\n");
		exit(1);
	}

	printf("Parent program ending\n");
	exit(1);
}
