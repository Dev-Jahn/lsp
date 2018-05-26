
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void ssu_signal_handler(int signo);

int main(void)
{
	/*When error while hanling signal*/
	if (signal(SIGINT, ssu_signal_handler) == SIG_ERR)
	{
		fprintf(stderr, "cannot handle SIGINT\n");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGTERM, ssu_signal_handler) == SIG_ERR)
	{
		fprintf(stderr, "cannot handler SIGTERM\n");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGPROF, ssu_signal_handler) == SIG_ERR)
	{
		fprintf(stderr, "cannot handler SIGPROF\n");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGHUP, ssu_signal_handler) == SIG_ERR)
	{
		fprintf(stderr, "cannot handler SIGHUP\n");
		exit(EXIT_FAILURE);
	}
	/*Wait for signal*/
	while(1)
		pause();
	exit(0);
}
static void ssu_signal_handler(int signo)
{
	/*Action for each signal*/
	if (signo==SIGINT)
		printf("caught SIGINT\n");
	else if (signo == SIGTERM)
		printf("caught SIGTERM\n");
	else
	{
		fprintf(stderr, "unexpected signal\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

