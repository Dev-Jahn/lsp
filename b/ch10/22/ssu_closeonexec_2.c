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
	/*get close-on-exec flag*/
	val = fcntl(fd, F_GETFD, 0);
	/*if on*/
	if (val&FD_CLOEXEC)
		printf("close-on-exec bit on\n");
	/*off*/
	else
		printf("close-on-exec bit off\n");
	/*set flag to on	*/
	val|=FD_CLOEXEC;
	if (val&FD_CLOEXEC)
		printf("close-on-exec bit on\n");
	else
		printf("close-on-exec bit off\n");
	/*apply flag to fd*/
	fcntl(fd, F_SETFD, val);

	/*replace text area and loop forever*/
	execl("./ssu_loop", "./ssu_loop", NULL);
	exit(0);
}
