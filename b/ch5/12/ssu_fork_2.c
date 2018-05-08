#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
	struct timeval begin, end;
	pid_t pid;
	char character, first, last;
	long i;
	//부모프로세스
	if ((pid = fork()) > 0)
	{
		first = 'A';
		last = 'Z';
	}
	//자식프로세스
	else if (pid == 0)
	{
		first = 'a';
		last = 'z';
	}
	else
	{
		fprintf(stderr, "%s\n", argv[0]);
		exit(1);
	}
	//first 부터 last까지의 문자를 출력
	for (character = first; character <= last; character++)
	{
	/*gettimeofday(&begin, NULL);*/
		/*for (i=0;i<=1000000;i++);*/

	/*gettimeofday(&end, NULL);*/
		write(1, &character, 1);
	}
	
	printf("\n");
	/*printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); */
	exit(0);
}

