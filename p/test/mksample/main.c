#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

int dircnt = 1;
int filecnt = 1;
int recnt = 1;
void make(int level, int max);
char *makedir(int level, int max);
void makefile();

int main(int argc, char *argv[])
{
	if (argc!=3)
	{
		printf("fuck\n");
		exit(1);
	}
	int level = atoi(argv[1]);	
	int max = atoi(argv[2]);
	mkdir ("sample", 0777);
	chdir("sample");
	make(level, max);
}
void make(int level, int max)
{
	srand(time(NULL));
	for (int i=0;i<=rand()%max;i++)
		makefile();
	for (int i=0;i<=rand()%max;i++)
		makedir(level, max);
}
char *makedir(int level, int max)
{ 
	char *name = (char*)malloc(256);
	char buf[256];
	strcpy(name, "dir");
	sprintf(buf, "%d", dircnt);
	strcat(name, buf);
	mkdir(name, 0777);
	dircnt++;
	if(recnt<level)
	{
		chdir(name);
		recnt++;
		make(level, max);
		chdir("..");
		recnt--;
	}
	return name;
} 
void makefile()
{
	srand(time(NULL));
	int fd, size = rand()%256+1;
	char name[256];
	char buf[256];
	strcpy(name, "file");
	sprintf(buf, "%d", filecnt);
	strcat(name, buf);
	memset(buf, 0, 256);
	for (int i =0;i<256;i++)
		strcat(buf,"Z");
	filecnt++;
	fd = open(name, O_RDWR|O_CREAT, 0666); 
	write(fd, buf, size);
	printf("file:%s\n", name);

	close(fd);
}
