#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFSIZE 1024
#define WORDMAX 100

int main(void)
{
	int fd;
	int length = 0, offset = 0, count = 0;
	char *fname = "ssu_test.txt";
	char buf[WORDMAX][BUFSIZE];
	int i;
	fd = open(fname, O_RDONLY);
	while((length = read(fd, buf[count], BUFSIZE))>0)
	{
		buf[count][length] = '\0';
		for(i=0;i<BUFSIZE;i++)
		{
			if (buf[count][i]=='\n')
			{
				buf[count][i+1] = '\0';
				if (i==0)
					break;
				offset = offset +i+1;
				lseek(fd, offset, SEEK_SET);
				count++;
			}
		}
	}
	close(fd);
	for (i=0;i<count;i++)
		printf("%s\n", buf[i]);
	printf("line number:%d\n", count);
	exit(0);
}
