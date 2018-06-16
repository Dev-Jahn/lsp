#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
	int fd;
	int val;
	/*create file*/
	if ((fd = open("exec_copy.txt", O_CREAT))<0)
	{
		fprintf(stderr, "open error for %s\n", "exec_copy.txt");
		exit(1);
	}
	/*get close-on-exec bit*/
	val = fcntl(fd, F_GETFD, 0);
	/*if on*/
	if (val&FD_CLOEXEC)
		printf("close-on-exec bit on\n");
	/*if off*/
	else
		printf("close-on-exec bit off\n");
	/*turn on the bit*/
	val|=FD_CLOEXEC;
	/*check it*/
	if(val&FD_CLOEXEC)
		printf("close-on-exec bit on\n");
	else
		printf("close-on-exec bit off\n");

	/*get close-on-exec bit*/
	fcntl(fd, F_SETFD, val);
	/*run blank loop*/
	execl("./loop", "./loop", NULL);
}

