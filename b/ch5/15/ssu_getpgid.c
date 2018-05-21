#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pgid;
	pid_t pid;

	if (argc<2)
	{
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		exit(1);
	}
	/*Get process id*/
	pid = getpid();
	/*Get process group id*/
	pgid = getpgid(atoi(argv[1]));
	/*Print*/
	printf("pid: %d, pgid: %d\n", pid, pgid);
	exit(0);
}
