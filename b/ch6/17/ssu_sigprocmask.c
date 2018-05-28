#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
	sigset_t sig_set;
	int count;

	/*Fill signal set with 0*/
	sigemptyset(&sig_set);
	/*Add SIGINT to set*/
	sigaddset(&sig_set, SIGINT);
	/*Block SIGINT*/
	sigprocmask(SIG_BLOCK, &sig_set, NULL);

	/*Wait for 3 second*/
	for (count=3;0<count;count--)
	{
		printf("count %d\n", count);
		sleep(1);
	}

	/*Unblock SIGINT*/
	printf("Ctrl-C에 대한 블록을 해제\n");
	sigprocmask(SIG_UNBLOCK, &sig_set, NULL);
	printf("count중 Ctrl-C입력하면 이 문장은 출력되지않음.\n");
	while(1);
	exit(0);
}
