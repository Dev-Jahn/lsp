#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char *fname = "ssu_test.txt";
	FILE *fp;
	long position;
	int character;
	//읽기모드로 파일스트림 열기
	if ((fp = fopen(fname, "r")) == NULL)
	{
		fprintf(stderr, "fopen error for %s\n", fname);
		exit(1);
	}
	//숫자 입력해서 position에 저장
	printf("Input number >>");
	scanf("%ld", &position);
	//position-1로 파일오프셋 이동
	fseek(fp, position - 1, SEEK_SET);
	//한글자 읽기
	character = getc(fp);
	//해당 글자 출력
	printf("%ldth character => %c\n", position, character);
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000 + end.tv_usec-begin.tv_usec); 
	exit(0);
}

