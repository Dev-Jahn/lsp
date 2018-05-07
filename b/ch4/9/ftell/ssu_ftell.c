#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char *fname = "ssu_test.txt";
	FILE *fp;
	long fsize;
	//읽기모드로 파일스트림 열기
	if ((fp = fopen(fname, "r")) == NULL)
	{
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}
	//파일끝으로 오프셋 이동
	fseek(fp, 0, SEEK_END);
	//현재 오프셋 fsize에 저장
	fsize = ftell(fp);
	//출력
	printf("The size of <%s> is %ld bytes\n", fname, fsize);
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000 + end.tv_usec-begin.tv_usec); 
	exit(0);
}
