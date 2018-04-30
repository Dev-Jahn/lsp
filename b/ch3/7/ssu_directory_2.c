#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>

#ifdef PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

#define MAX_PATH_GUESSED 1024

#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

char *pathname;
char command[LINE_MAX], grep_cmd[LINE_MAX];

int ssu_do_grep(void)
{
	struct dirent *dirp;
	struct stat statbuf;
	char *ptr;
	DIR *dp;
	//파일속성읽기
	if (lstat(pathname, &statbuf)<0)
	{
		fprintf(stderr, "lstat error for %s\n", pathname);
		return 0;
	}
	//디렉토리가 아니면
	if (S_ISDIR(statbuf.st_mode)==0)
	{
		//실행할 명령 출력
		sprintf(command, "%s %s", grep_cmd, pathname);
		//파일명 출력
		printf("%s : \n", pathname);
		//grep 실행
		system(command);
		return 0;
	}
	//pathname의 끝으로 포인터이동
	ptr = pathname + strlen(pathname);
	//'/'문자 삽입
	*ptr++ = '/';
	//문자열 종료
	*ptr = '\0';
	//디렉토리 초기화
	if ((dp=opendir(pathname))==NULL)
	{
		fprintf(stderr, "opendir error for %s\n", pathname);
		return 0;
	}
	//엔트리 읽기
	while ((dirp=readdir(dp))!=NULL)
		//현재디렉토리, 부모디렉토리 외의 디렉토리가 있으면
		if (strcmp(dirp->d_name, ".") &&strcmp(dirp->d_name, ".."))
		{
			//경로 문자열 뒤에 디렉토리명 concat.
			strcpy(ptr, dirp->d_name);
			//재귀호출
			if (ssu_do_grep()<0)
				break;
		}
		//끝문자 하나 제거
		ptr[-1] = 0;
		//디렉토리 구조체사용종료
		closedir(dp);
		return 0;
}

void ssu_make_grep(int argc, char *argv[])
{
	int i;
	//grep 파일명 으로 구성된 명령어 문자열 구성
	strcpy(grep_cmd, " grep");
	for (i = 1;i<argc-1;i++)
	{
		strcat(grep_cmd, " ");
		strcat(grep_cmd, argv[i]);
		struct stat a;
	}
}

int main(int argc, char *argv[])
{
	//인자형식 불일치시 에러
	if  (argc <2)
	{
		fprintf(stderr, "usage: %s <-CVbchilnsvwx> <-num> <-A num> <-B num> <-f file> \n""		<-e> expr <directory>\n",argv[0]);
		exit(1);
	}
	if (pathmax==0)
	{
		if ((pathmax=pathconf("/", _PC_PATH_MAX))<0)
			pathmax = MAX_PATH_GUESSED;
		else
			pathmax++;
	}
	if ((pathname=(char*)malloc(pathmax+1))==NULL)
	{
		fprintf(stderr, "malloc error\n");
		exit(1);
	}
	strcpy(pathname, argv[argc-1]);
	//입력받은 인자들에 대해 make_grep 실행
	ssu_make_grep(argc, argv);
	//전역 grep_cmd 배열로 grep
	ssu_do_grep();
	exit(0);
}

