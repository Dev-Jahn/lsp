#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char operator;
	FILE *fp;
	int character;
	int number = 0;
	//읽기모드로 파일스트림 오픈
	if ((fp = fopen("ssu_expr.txt", "r")) == NULL)
	{
		fprintf(stderr, "fopen error for ssu_expr.txt\n");
		exit(1);
	}
	//파일 끝이 아닌동안 반복
	while (!feof(fp))
	{
		//한글자 읽고 숫자이면 number에 누적
		while((character=fgetc(fp))!=EOF && isdigit(character))
			number = 10 * number + character - 48;
		//number출력
		fprintf(stdout, " %d\n", number);
		//초기화
		number = 0;
		//연산자이면
		if (character != EOF)
		{
			//스트림에 되돌리기
			ungetc(character, fp);
			//연산자에 저장
			operator = fgetc(fp);
			//출력
			printf("Operator => %c\n", operator);
		}
	}

	fclose(fp);
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}

