#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	//생성할 파일의 이름
	char *fname="ssu_test.txt";
	//생성후 리턴되는 파일디스크립터를 저장할 변수
	int fd;
	/*
	 *creat의 리턴이 -1일때의 예외처리
	 *0666은 (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)와 동일
	 */
	if ((fd=creat(fname,0666))<0)
	{
		//표준에러스트림에 문자열 출력
		fprintf(stderr,"creat error for %s\n",fname);
		exit(1);
	}
	//성공시
	else
	{
		//표준출력에 파일이름, 디스크립터 번호 출력
		printf("Success!\nFilename: %s\nDescriptor: %d\n",fname, fd);
		//사용끝난 파일 닫기
		close(fd);
	}
	//종료
	exit(0);
}

