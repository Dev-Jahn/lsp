#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

void ssu_signal_handler(int signo);

jmp_buf jump_buffer;

int main(void)
{
	/*Set signal handler*/
	signal(SIGINT, ssu_signal_handler);

	while(1)
	{
		if (setjmp(jump_buffer) == 0)
		{
			/*Wait for signal*/
			printf("Hit Ctrl-c at anytime ... \n");
			pause();
		}
	}

	exit(0);
}

void ssu_signal_handler(int signo)
{
	char character;
	/*Reset handler*/
	signal(signo, SIG_IGN);
	printf("Did you hit Ctrl-c?\n" "Do you really want to quit? [y/n] ");
	/*Get input*/
	character = getchar();
	/*When yes	*/
	if (character == 'y'||character == 'Y')
		exit(0);
	/*When no*/
	else
	{
		/*Set handler*/
		signal(SIGINT, ssu_signal_handler);
		/*Goto waiting state*/
		longjmp(jump_buffer, 1);
	}
}
