#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo);

int count = 0;
int main(void)
{
	/*Set signal handler*/
	signal(SIGALRM, ssu_signal_handler);

	/*Raise SIGALRM after 1sec*/
	alarm(1);

	/*Wait for signal*/
	while(1);
	
	exit(0);
}
			
void ssu_signal_handler(int signo)
{
	/*Print*/
	printf("alarm %d\n", count++);
	/*Raise SIGALRM after 1sec*/
	alarm(1);
}
