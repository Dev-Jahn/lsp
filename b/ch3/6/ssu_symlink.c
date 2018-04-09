#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
	//시간구조체 선언
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	//인자형태 불일치시 에러
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s <actualname> <symname>\n", argv[0]);
		exit(1);
	}
	//심볼릭링크 생성
	if (symlink(argv[1], argv[2]) < 0)
	{
		fprintf(stderr, "symlink error\n");
		exit(1);
	}
	else
		printf("symlink: %s -> %s\n", argv[2], argv[1]);
	//시간계산
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}


