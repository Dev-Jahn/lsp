#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	if (argc!=2)
	{
		fprintf(stderr, "wrong arg\n");
		exit(1);
	}
	/*umask(0022);*/
	int fd = open(argv[1], O_RDWR|O_CREAT|O_TRUNC, 0666);
	/*int fd = open(argv[1], O_RDWR);*/
	close(fd);
}
