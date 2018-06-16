#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
int main(void)
{
	srand(time(NULL));
	int fd = open("ssu_test1.txt", O_CREAT|O_TRUNC|O_WRONLY, 0666);
	char buf[500000];
	int a;
	for (int i=0;i<500000;i++)
	{
		a = rand()%10;
		sprintf(buf+i, "%d", a);
	}
	write(fd, buf, 500000);
}
