#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

void ssu_local_func(void);

int main(void)
{
	printf("Before vfork, pid:%d\n", getpid());
	ssu_local_func();
	printf("After ssu_local_func, my PID is %d\n", getpid());
	exit(0);
}

void ssu_local_func(void)
{
	pid_t pid;
	//자식프로세스에서 pid출력
	if ((pid = vfork()) == 0)
	{
		printf("I'm child. My PID is %d\n", getpid());
	}
	//부모는 3초 sleep
	else if (pid > 0)
		sleep(3);
	else
		fprintf(stderr, "vfork error\n");
}
