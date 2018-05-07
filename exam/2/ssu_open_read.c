#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

int main(void)
{
	char *fname = "ssu_test.txt";
	int fd, fd2, len, size;
	char buf[BUFFER_SIZE];

	if ((fd = open(fname, O_RDONLY)) <0)
	{
		fprintf(stderr, "open error");
		exit(1);
	}
	if ((size = lseek(fd, 0, SEEK_END))<0)
	{
		fprintf(stderr, "lseek error");
		exit(1);
	}
	printf("%d\n", size);
	lseek(fd, 0, SEEK_SET);
	len = read(fd, buf, BUFFER_SIZE);
	fd2 = dup(1);
	write(fd2, buf, len);
	exit(0);
	close(fd);
}

