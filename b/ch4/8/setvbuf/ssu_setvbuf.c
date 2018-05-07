#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024

void ssu_setbuf(FILE *fp, char *buf);
int main(void)
{
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	char buf[BUFFER_SIZE];
	//현재 열린 터미널
	char *fname = "/dev/pts/2";
	FILE *fp;
	//터미널 열기
	if((fp=fopen(fname, "w")) == NULL)
	{
		fprintf(stderr, "fopen error for %s", fname);
		exit(1);
	}
	
	ssu_setbuf(fp, buf);		//버퍼설정
	fprintf(fp, "Hello,  ");	//버퍼에 삽입
	sleep(1);
	fprintf(fp, "UNIX!!");	//버퍼에 삽입
	sleep(1);
	fprintf(fp, "\n");		//버퍼에 삽입
	sleep(1);
	ssu_setbuf(fp, NULL);		//버퍼해제
	fprintf(fp, "HOW");			//즉시출력
	sleep(1);
	fprintf(fp, " ARE");		//즉시출력
	sleep(1);
	fprintf(fp, " YOU?");		//즉시출력
	sleep(1);
	fprintf(fp, "\n");			//즉시출력
	sleep(1);

	gettimeofday(&end, NULL);
	printf("Elapsed time:%ldus\n",end.tv_usec-begin.tv_usec); 
	exit(0);
}

void ssu_setbuf(FILE *fp, char *buf)
{
	size_t size;
	int fd;
	int mode;

	fd = fileno(fp);
	//터미널에 연결된 디스크립터인지 확인(표준입출력)
	if (isatty(fd))
		mode = _IOLBF;	//line-buffered로 설정
	//파일이면
	else
		mode = _IOFBF;	//full-buffered로 설정
	//버퍼인자가 NULL이면
	if (buf==NULL)
	{
		mode = _IONBF;	//non-buffered로 설정
		size=0;
	}
	else
		size = BUFFER_SIZE;
	//버퍼설정
	setvbuf(fp, buf, mode, size);
}


