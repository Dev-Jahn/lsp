#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigint_handler(int signo)
{
	printf("\nSIGINT handler!\n");
	exit(1);
}

int main(int argc, char *argv[])
{
	int i;
	/*begin*/
	struct sigaction act;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	act.sa_handler = sigint_handler;
	sigaction(SIGINT, &act, NULL);
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	/*end*/
	while(1)
	{
		/*add*/
		sigprocmask(SIG_BLOCK, &set, NULL);
		/*add*/
		for (i=0;i<argc;i++)
			printf("ARGV[%d]:%s\n", i, argv[i]);
		sleep(1);
		/*add*/
		sigprocmask(SIG_UNBLOCK, &set, NULL);
		/*add*/
	}
	exit(0);
}

