#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH_MAX 1024

int main(void)
{
	char *pathname;
	//작업디렉토리 홈디렉토리로 변경
	if (chdir("/home/jahn")<0)
	{
		fprintf(stderr, "chdir error\n");
		exit(1);
	}
	//path 변수 할당
	pathname = malloc(PATH_MAX);
	//pathname에 현재 작업 디렉토리 저장
	if (getcwd(pathname, PATH_MAX)==NULL)
	{
		fprintf(stderr, "getcwd error \n");
		exit(1);
	}
	//현재 작업 디렉토리 출력
	printf("current directory =%s\n", pathname);
	exit(0);
}

