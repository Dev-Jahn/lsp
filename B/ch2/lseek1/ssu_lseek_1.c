#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
	//파일이름
	char *fname = "ssu_test.txt";
	//파일오프셋
	off_t fsize;
	//파일디스크립터
	int fd;
	
	//파일열기, 에러시 에러출력
	if ((fd=open(fname, O_RDONLY))<0)
	{
		fprintf(stderr,"open error for %s\n",fname);
		exit(1);
	}
	/*
	 *파일을 연 직후이므로 오프셋이 0인상태
	 *whence가 SEEK_END이므로 파일크기+0(오프셋인자)로 오프셋 설정
	 */
	if ((fsize=lseek(fd,(off_t)0, SEEK_END))<0)
	{
		//에러시 에러출력
		fprintf(stderr,"lseek error\n");
		exit(1);
	}
	//현재 오프셋==파일크기
	//파일크기 출력
	printf("The size of <%s> is %ld bytes.\n", fname, fsize);

	exit(0);
}
