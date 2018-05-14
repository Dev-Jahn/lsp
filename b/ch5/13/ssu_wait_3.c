#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	//자식프로세스에서 echo
	if (fork() == 0)
		execl("/bin/echo", "echo", "this is", "message one", (char *)0);

	//자식프로세스에서 echo
	if (fork() == 0)
		execl("/bin/echo", "echo", "this is", "message Two", (char *)0);

	printf("parent: waiting for children\n");
	//리턴이 -1이 아니면 정상진행
	while (wait((int *)0) != -1);

	printf("parent: all children terminated\n");
	exit(0);
}
