#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main()
{
	//문자열을 저장할 버퍼
	char buf[BUFFER_SIZE];
	char *fname = "ssu_test.txt";
	int count;
	int fd1, fd2;

	//같은 파일을 두번 open해 두개의 디스크립터를 할당
	fd1 = open(fname, O_RDONLY, 0644);
	fd2 = open(fname, O_RDONLY, 0644);
	//둘중 하나라도 에러가 있으면 처리
	if (fd1 < 0 || fd2 < 0)
	{
		fprintf(stderr, "open error for %s\n", fname);
		exit(1);
	}

	//25바이트 읽어 버퍼에 저장후 오프셋 저장
	count = read(fd1, buf, 25);
	//버퍼끝에 NULL문자 삽입
	buf[count] = 0;
	//출력
	printf("fd1 first: %s\n", buf);
	//개행문자 건너뛰기
	lseek(fd1, 1, SEEK_CUR);
	//24바이트 읽기
	count = read(fd1, buf, 24);
	//문자열 종료
	buf[count] = 0;
	//출력
	printf("fd1 second: %s\n", buf);

	//fd1과 동일
	count = read(fd2, buf, 25);
	buf[count] = 0;
	printf("fd2 first: %s\n", buf);
	lseek(fd2, 1, SEEK_CUR);
	count = read(fd2, buf, 24);
	buf[count] = 0;
	printf("fd2 second: %s\n", buf);
	
	exit(0);
}
