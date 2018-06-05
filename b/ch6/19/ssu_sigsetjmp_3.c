#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>

static void ssu_signal_handler1(int signo);
static void ssu_signal_handler2(int signo);

sigjmp_buf jmp_buf1;
sigjmp_buf jmp_buf2;

int main(void)
{
	struct sigaction act_sig1;
	struct sigaction act_sig2;
	int i, ret;

	printf("My PID is %d\n", getpid());
	/*Get return value of sigsetjmp, save signal mask*/
	ret = sigsetjmp(jmp_buf1, 1);

	/*Before jump*/
	if (ret==0)
	{
		act_sig1.sa_handler = ssu_signal_handler1;
		sigaction(SIGINT, &act_sig1, NULL);
	}
	/*After jump*/
	else if (ret ==3)
		printf("------------\n");

	printf("Starting\n");
	/*Set second jump location*/
	sigsetjmp(jmp_buf2, 2);
	/*set handler*/
	act_sig2.sa_handler = ssu_signal_handler2;
	sigaction(SIGUSR1, &act_sig2, NULL);

	/*print count for 20secs*/
	for (i=0;i<20;i++)
	{
		printf("i = %d\n", i);
		sleep(1);
	}

	exit(0);
}

static void ssu_signal_handler1(int signo)
{
	/*When interrupted, jump to first location*/
	fprintf(stderr, "\nInterrupted\n");
	siglongjmp(jmp_buf1, 3);
}

static void ssu_signal_handler2(int signo)
{
	/*When signaled, jump to second location*/
	fprintf(stderr, "\nSIGUSR1\n");
	siglongjmp(jmp_buf2, 2);
}
