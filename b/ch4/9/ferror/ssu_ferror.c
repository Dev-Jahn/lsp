#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	FILE *fp;
	int character;
	//인자 불일치시 에러
	if(argc != 2)
	{
		fprintf(stderr, "usage: %s <filename>\n", argv[0]);
		exit(1);
	}
	//읽기모드로 파일스트림 오픈
	if ((fp=fopen(argv[1], "r"))==NULL)
	{
		fprintf(stderr, "fopen error for %s\n", argv[1]);
		exit(1);
	}
	//한글자 읽어서 저장
	character = fgetc(fp);
	//파일의 끝이 아닌동안 반복
	while(!feof(fp))
	{
		//표준출력으로 출력
		fputc(character, stdout);
		//입출력 에러 발생시 에러출력
		if (ferror(fp))
		{
			fprintf(stderr,  "Error detected!!\n");
			clearerr(fp);
		}
		//한글자 읽어서 저장
		character = fgetc(fp);
	}
	fclose(fp);
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}
