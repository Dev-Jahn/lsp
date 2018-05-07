#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char buf[BUFFER_SIZE];
	FILE *fp;
	//인자형식 불일치시 에러
	if (argc != 2)
	{
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		exit(1);
	}
	//읽기,쓰기 로 파일스트림 오픈
	if ((fp = fopen(argv[1], "w+")) == NULL)
	{
		fprintf(stderr, "fopen error for %s\n", argv[1]);
		exit(1);
	}
	//프롬프트 출력
	fputs("Input String >> ", stdout);
	//표준입력에서 한줄입력
	gets(buf);
	//버퍼내용 파일로  출력
	fputs(buf, fp);
	//오프셋 시작으로 재설정
	rewind(fp);
	//파일에서 한줄읽어 버퍼에 저장
	fgets(buf, sizeof(buf), fp);
	//표준 출력
	puts(buf);
	//닫기
	fclose(fp);
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",(end.tv_sec-begin.tv_sec)*1000000 + end.tv_usec-begin.tv_usec); 
	exit(0);
}

