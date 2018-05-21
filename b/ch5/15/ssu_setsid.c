#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;
	if ((pid=fork())<0)
	{
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	/*Terminate parent process*/
	else if (pid!=0)
		exit(1);
	printf("before pid = %d, sid = %d\n", getpid(), getsid(getpid()));
	/*Make new session and become session leader*/
	setsid();
	printf("after pid = %d, sid = %d\n", getpid(), getsid(getpid()));
	exit(0);
}
