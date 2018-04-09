#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
	//시간 측정할 시간 구조체
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	//인자부족시 에러
	if (argc!=3)
	{
		fprintf(stderr, "usage: %s <oldname> <newname> \n", argv[0]);
		exit(1);
	}
	//같은 inode로 링크된 파일생성
	if (link(argv[1], argv[2])<0)
	{
		fprintf(stderr, "link error \n");
		exit(1);
	}
	else
		printf("step1 passed.\n");
	//원본삭제
	if (remove(argv[1])<0)
	{
		fprintf(stderr, "remove error\n");
		remove(argv[2]);
		exit(1);
	}
	else
		printf("step2 passed.\n");

	printf("Success!\n");
	//경과시간 계산후 출력
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}
