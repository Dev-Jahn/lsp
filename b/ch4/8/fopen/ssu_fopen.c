#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char *fname = "ssu_test.txt";
	char *mode = "r";
	//파일스트림 열기
	if (fopen(fname, mode) == NULL)
	{
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}
	//성공시 출력
	else
		printf("Success!\nFilename: <%s>, mode: <%s>\n", fname, mode);
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}
