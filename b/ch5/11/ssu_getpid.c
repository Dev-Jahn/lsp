#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	//현재 프로세스 ID 출력
	printf("Process ID		= %d\n", getpid());
	//부모 프로세스 ID 출력
	printf("Parent process ID	= %d\n", getppid());
	//RUID 출력
	printf("Real user ID		= %d\n", getuid());
	//EUID 출력
	printf("Effective user ID	= %d\n", geteuid());
	//RGID 출력
	printf("Real group ID		= %d\n", getgid());
	//EGID 출력
	printf("Effective group ID	= %d\n", getegid());

	exit(0);
}
