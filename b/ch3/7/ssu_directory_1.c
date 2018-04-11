#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h> 
#define DIRECTORY_SIZE MAXNAMLEN

int main(int argc, char *argv[])
{
	struct dirent *dentry;
	struct stat statbuf;
	char filename[DIRECTORY_SIZE+1];
	DIR *dirp;
	//인자 없을때 에러
	if (argc<2)
	{
		fprintf(stderr, "usage: %s <directory>\n", argv[0]);
		exit(1);
	}
	//인자로 받은 디렉토리 구조체 초기화, 작업디렉토리 변경
	if ((dirp=opendir(argv[1]))==NULL||chdir(argv[1])==-1)
	{
		fprintf(stderr, "opendir, chdir error for %s\n", argv[1]);
		exit(1);
	}
	//디렉토리구조체에서 엔트리 읽어오기
	while((dentry=readdir(dirp))!=NULL)
	{
		if (dentry->d_ino ==0)
			continue;
		//파일이름 filename 에 복사.
		memcpy(filename, dentry->d_name, DIRECTORY_SIZE);
		//stat으로 파일속성읽기
		if (stat(filename, &statbuf)==-1)
		{
			fprintf(stderr, "stat error for %s\n", filename);
			break;
		}
		//일반 파일이면 이름, 크기 출력
		if ((statbuf.st_mode & S_IFMT)==S_IFREG)
			printf("%-14s %ld\n", filename, statbuf.st_size);
		//아니면 이름 출력
		else
			printf("%-14s\n", filename);
	}
	exit(0);
}
