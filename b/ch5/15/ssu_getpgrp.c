#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	pid_t pgid;
	pid_t pid;

	/*Get process id*/
	pid = getpid();
	/*Get process group id*/
	pgid = getpgrp();
	printf("pid: %d, pgid: %d\n", pid, pgid);
	exit(0);
}
