#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
	//디스크립터, 시간구조체 선언
	int fd;
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	//인자부족시 에러	
	if(argc!=3)
	{
		fprintf(stderr, "usage: %s <oldname> <newname>\n", argv[0]);
		exit(1);
	}
	//읽기전용으로 열기
	if ((fd=open(argv[1],O_RDONLY))<0)
	{
		fprintf(stderr, "first open error for %s\n", argv[1]);
		exit(1);
	}
	//열리는지 확인 후 닫기
	else
		close(fd);
	//이름변경
	if (rename(argv[1], argv[2])<0)
	{
		fprintf(stderr, "rename error\n");
		exit(1);
	}
	//원래이름으로 열기	
	if ((fd=open(argv[1], O_RDONLY))<0)
		printf("second open error for %s\n", argv[1]);
	else
	{
		fprintf(stderr, "it's very strange!\n");
		exit(1);
	}
	//바꾼이름으로 열기	
	if ((fd = open(argv[2], O_RDONLY))<0)
	{
		fprintf(stderr, "third open error for %s\n", argv[2]);
		exit(1);
	}
	//이상없으면 출력
	printf("Everything is good!\n");
	//경과시간계산
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}


