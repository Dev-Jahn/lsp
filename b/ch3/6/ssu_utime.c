#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <utime.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
	//변수선언
	struct utimbuf time_buf;
	struct stat statbuf;
	int fd;
	int i;
	struct timeval begin, end;
	//시작시간 측정
	gettimeofday(&begin, NULL);
	//입력된 인자전체에 대해 반복
	for (i = 1;i<argc;i++)
	{
		//파일속성 읽어서 버퍼에 저장
		if (stat(argv[i], &statbuf)<0)
		{
			fprintf(stderr, "stat error for %s\n", argv[i]);
			continue;
		}
		//열기
		if ((fd = open(argv[i], O_RDWR|O_TRUNC))<0)
		{
			fprintf(stderr, "open error for %s\n", argv[i]);
			continue;
		}
		//닫기
		close(fd);
		//열기전의 접근시간 구조체에 복사 
		time_buf.actime = statbuf.st_atime;
		//열기전의 수정시간 구조체에 복사
		time_buf.modtime = statbuf.st_mtime;
		//구조체내의 시간으로 현재파일 속성 변경
		if (utime(argv[i], &time_buf)<0)
		{
			fprintf(stderr, "utime error for %s\n", argv[i]);
			continue;
		}
	}
	//종료시간측정, 계산
	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}
