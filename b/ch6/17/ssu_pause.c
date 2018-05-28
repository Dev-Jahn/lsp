#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_alarm(int signo);

int main(void)
{
	printf("Alarm Setting\n");
	/*Set signal handler*/
	signal(SIGALRM, ssu_alarm);
	/*Raise SIGALRM after 2sec*/
	alarm(2);

	while (1)
	{
		printf("done\n");
		/*Pause and wait SIGALRM*/
		pause();
		/*Raise SIGALRM after 2sec*/
		alarm(2);
	}

	exit(0);
}

void ssu_alarm(int signo)
{
	printf("alarm..!!!\n");
}
