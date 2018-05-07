#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	int character;
	//EOF를 만나기 전까지 표준입력에서 한글자씩 받아 character에 할당
	while ((character = getc(stdin))!=EOF)
		if (putc(character, stdout) == EOF)		//표준출력으로 출력
		{
			fprintf(stderr, "standard output error\n");
			exit(1);
		}
	//입출력 에러여부 확인
	if (ferror(stdin))
	{
		fprintf(stderr, "standard input error\n");
		exit(1);
	}
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}

