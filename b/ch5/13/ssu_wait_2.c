#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define EXIT_CODE 1

int main(void)
{
	pid_t pid;
	int ret_val, status;
	//자식프로세스에서 pid출력
	if ((pid = fork())==0)
	{
		printf("child: pid = %d ppid = %d exit_code = %d\n", getpid(), getppid(), EXIT_CODE);
		exit(EXIT_CODE);
	}
	
	printf("parent: waiting for child = %d\n", pid);
	//종료대기
	ret_val = wait(&status);
	printf("parent: return value = %d, ", ret_val);
	printf(" child's status = %x", status);
	//wait리턴의 하위 8비트 출력
	printf(" and shifted = %x\n", (status >> 8));
	exit(0);
}
