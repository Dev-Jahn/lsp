#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void search_dir(char *dir)
{
	struct dirent *dentry;
	struct stat statbuf;
	DIR *dirp;
	char cwd[1024];

	getcwd(cwd, 1024);

	if(lstat(dir, &statbuf)<0)
	{
		fprintf(stderr, "lstat error\n");
		exit(1);
	}

	if(!S_ISDIR(statbuf.st_mode))
	{
		printf("%s/%s\n", cwd, dir);		//dir아닌가
		return;
	}

	if ((dirp = opendir(dir))==NULL)
	{
		fprintf(stderr, "opendir error\n");
		exit(1);
	}

	chdir(dir);
	while ((dentry = readdir(dirp))!=NULL)
	{
		if (dentry->d_ino == 0||strcmp(dentry->d_name,"..")==0||strcmp(dentry->d_name, ".")==0)
			continue;
		search_dir(dentry->d_name);
	}
	char cwd2[1024];
	if(strcmp(getcwd(cwd2,1024),cwd)!=0)
		printf("%s/%s\n", cwd, dir);
	chdir(cwd);
}
int main(int argc, char *argv[])
{
	if (argc <2)
	{
		fprintf(stderr, "arg wrong\n");
		exit(1);
	}
	search_dir(argv[1]);
}
