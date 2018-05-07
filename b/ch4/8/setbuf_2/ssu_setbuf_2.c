#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char buf[BUFFER_SIZE];

	setbuf(stdout, buf);	//버퍼설정
	printf("Hello, ");		//버퍼에 삽입
	sleep(1);
	printf("OSLAB!!");		//버퍼에 삽입
	sleep(1);
	printf("\n");			//버퍼에 삽입, 개행이므로 출력
	sleep(1);

	setbuf(stdout, NULL);	//버퍼해제
	printf("How");			//즉시출력
	sleep(1);
	printf(" are");			//즉시출력
	sleep(1);
	printf(" you?");		//즉시출력
	sleep(1);
	printf("\n");			//즉시출력
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}
