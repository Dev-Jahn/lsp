#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
	int fd;
	int val;
	fd = open("test.txt", O_CREAT);
	val = fcntl(fd, F_GETFD, 0);
	if (val&FD_CLOEXEC)
		printf("close-on-exec bit on\n");
	else
		printf("close-on-exec bit off\n");
	
	val|=FD_CLOEXEC;
	if (val&FD_CLOEXEC)
		printf("close-on-exec bit on\n");
	else
		printf("close-on-exec bit off\n");
	fcntl(fd, F_SETFD, val);

	execl("./ssu_loop", "./ssu_loop", NULL);
	exit(0);
}
