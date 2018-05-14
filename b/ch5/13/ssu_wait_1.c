#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void ssu_echo_exit(int status);

int main(void)
{
	pid_t pid;
	int status;
	//실패시 에러
	if ((pid = fork()) < 0)
	{
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	//자식에서 exit(7)
	else if (pid == 0)
		exit(7);
	//wait에러
	if (wait(&status) != pid)
	{
		fprintf(stderr, "wait error\n");
		exit(1);
	}
	//exitstatus 출력
	ssu_echo_exit(status);

	if ((pid = fork()) < 0)
	{
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	//자식에서 abort()
	else if (pid == 0)
		abort();

	if (wait(&status) != pid)
	{
		fprintf(stderr, "wait error\n");
		exit(1);
	}

	//exitstatus 출력
	ssu_echo_exit(status);

	if ((pid = fork()) <0)
	{
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	//자식에서 0으로 나누기
	else if(pid == 0)
		status /= 0;

	if (wait(&status) != pid)
	{
		fprintf(stderr, "wait error\n");
		exit(1);
	}

	//exitstatus 출력
	ssu_echo_exit(status);
	exit(0);
}

void ssu_echo_exit(int status)
{
	if (WIFEXITED(status))
		printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP
				WCOREDUMP(status) ? " (core file generated)" : "");
#else
				"");
#endif
	else if (WIFSTOPPED(status))
		printf("child stopped, signal number = %d\n", WSTOPSIG(status));
}
