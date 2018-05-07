#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char buf[BUFFER_SIZE];
	int a, b;
	int i;

	setbuf(stdin, buf);		//버퍼설정
	scanf("%d %d", &a, &b); //입력받음
	//개행 전까지 버퍼내용 출력
	for (i=0;buf[i] != '\n'; i++)
		putchar(buf[i]);
	//개행출력
	putchar('\n');
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}
