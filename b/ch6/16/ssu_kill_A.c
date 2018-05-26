#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[])
{
	/*Usage error*/
	if (argc!=2)
	{
		fprintf(stderr, "usage: %s [Process ID]\n", argv[0]);
		exit(1);
	}
	/*Send SIGKILL to given pid*/
	else
		kill(atoi(argv[1]), SIGKILL);
	exit(0);
}
