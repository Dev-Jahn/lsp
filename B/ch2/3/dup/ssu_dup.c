#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(void)
{
	//문자열을 저장할 버퍼
	char buf[BUFFER_SIZE];
	//오픈할 파일이름
	char *fname = "ssu_test.txt";
	int count;
	int fd1, fd2;

	//파일 읽기로 열기
	if ((fd1 = open(fname, O_RDONLY,0644))<0)
	{
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}
	//fd2에 fd1 디스크립터 복제
	fd2 = dup(fd1);
	//fd1에서 12바이트 읽기
	count = read(fd1, buf, 12);
	//널문자 삽입
	buf[count] = 0;
	//출력
	printf("fd1's printf : %s\n", buf);
	//개행 건너뛰기
	lseek(fd1, 1, SEEK_CUR);
	//fd2에서 12바이트 읽기
	count = read(fd2, buf, 12);
	//널문자 삽입
	buf[count] = 0;
	//출력
	printf("fd2's printf : %s\n", buf);
	exit(0);
}
