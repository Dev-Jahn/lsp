#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024

int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char *fname = "ssu_test.dat";
	char name[BUFFER_SIZE];
	FILE *fp;
	int age;
	//읽기로 파일스트림 열기	
	fp = fopen(fname, "r");
	//이름, 나이 형식에 맞게 입력받아 저장
	fscanf(fp, "%s%d", name, &age);
	//스트림 닫기
	fclose(fp);
	//쓰기로 파일스트림 열기
	fp = fopen(fname, "w");
	//형식에 맞게 출력
	fprintf(fp, "%s is %d years old\n", name, age);
	//닫기
	fclose(fp);
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000 + end.tv_usec-begin.tv_usec); 
	exit(0);
}

