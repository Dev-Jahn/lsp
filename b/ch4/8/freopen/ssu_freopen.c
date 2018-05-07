#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char *fname = "ssu_test.txt";
	int fd;

	printf("First printf : Hello, OSLAB!!\n");
	//파일열기
	if ((fd = open(fname, O_RDONLY))<0)
	{
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}
	//표준출력을 닫고 해당 스트림구조체에 fname파일 할당	
	if (freopen(fname, "w", stdout)!=NULL)
		//파일에 출력
		printf("Second printf : Hello, OSLAB!!\n");

	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}
