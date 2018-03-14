#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

/*
 *메인함수
 */
int main()
{
	//open할 파일이름을 저장할 변수
	char *fname = "ssu_test.txt";
	//리턴받은 파일디스크립터를 저장할 변수
	int fd;
	//open의 리턴이 -1일때의 예외처리
	if ((fd=open(fname, O_RDONLY))<0)
	{
		//표준에러스트림으로 문자열을 출력
		fprintf(stderr, "open error for %s\n", fname);
		//exit status 1을 리턴하며 종료
		exit(1);
	}
	else
		//표준출력으로 성공메시지 출력
		printf("Success!\nFilename : %s\nDescriptor : %d\n", fname, fd);
	//종료
	exit(0);
}
