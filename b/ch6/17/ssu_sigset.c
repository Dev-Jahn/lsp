#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(void)
{
	sigset_t set;
	/*Fill signal set with 0*/
	sigemptyset(&set);
	/*Add SIGINT to set*/
	sigaddset(&set, SIGINT);

	switch (sigismember(&set, SIGINT))
	{
		case 1:
			printf("SIGINT is include. \n");
			break;
		case 0:
			printf("SIGINT is not include. \n");
			break;
		default:
			printf("failed to call sigismember()\n");
	}

	switch (sigismember(&set, SIGSYS))
	{
		case 1:
			printf("SIGSYS is included. \n");
			break;
		case 0:
			printf("SIGSYS is not included. \n");
			break;
		default:
			printf("failed to call sigismember() \n");
	}

	exit(0);
}
