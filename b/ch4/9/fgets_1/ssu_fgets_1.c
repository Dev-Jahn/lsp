#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char buf[BUFFER_SIZE];

	//표준입력으로 입력
	while(fgets(buf, BUFFER_SIZE, stdin) != NULL)
		//표준출력으로 출력
		if (fputs(buf, stdout) == EOF)
		{
			fprintf(stderr, "standard output error\n");
			exit(1);
		}
	//입력에러 발생시 에러출력
	if (ferror(stdin))
	{
		fprintf(stderr, "standard input error\n");
		exit(1);
	}
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}

