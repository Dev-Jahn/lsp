#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char *fname = "ssu_test.txt";
	FILE *fp;
	//파일스트림 열기
	if ((fp = fopen(fname, "r"))==NULL)
	{
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}
	//성공시 문자열 출력
	else
	{
		printf("Success!\n");
		printf("Opening \"%s\" in \"r\" mode!\n", fname);
	}
	//파일스트림 닫기
	fclose(fp);
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}
