#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main()
{
	char buf[BUFFER_SIZE];
	char *fname = "ssu_test.txt";
	int count;
	int fd1, fd2;

	fd1 = open(fname, O_RDONLY, 0644);
	fd2 = open(fname, O_RDONLY, 0644);

	if (fd1 < 0 || fd2 < 0)
	{
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	count = read(fd1, buf, 25);
	buf

