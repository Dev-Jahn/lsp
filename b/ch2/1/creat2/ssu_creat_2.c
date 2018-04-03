#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	//파일이름
	char *fname = "ssu_test.txt";
	//파일디스크립터 변수
	int fd;
	/*
	 *rw_rw_rw_권한으로 파일생성
	 *실패시 에러출력
	 */
	if ((fd=creat(fname, 0666))<0)
	{
		fprintf(stderr,"creat error for %s\n", fname);
		exit(1);
	}
	//성공시
	else
	{
		//파일닫기
		close(fd);
		//파일열기.읽기쓰기 가능
		fd=open(fname, O_RDWR);
		printf("Success!\n<%s> is new readable and writable\n",fname);
	}
	exit(0);
}
