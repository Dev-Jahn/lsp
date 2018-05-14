#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/wait.h>

double ssu_maketime(struct timeval *time);
void term_stat(int stat);

void ssu_print_child_info(int stat, struct rusage *rusage);

int main(void)
{
	struct rusage rusage;
	pid_t pid;
	int status;
	//프로세스 생성
	if ((pid = fork()) == 0)
	{
		//루트 디렉토리부터 stdio.h를 find 하는 명령실행
		char *args[] = {"find", "/", "-maxdepth", "4", "-name", "stdio.h", NULL};
		if(execv("/usr/bin/find", args)<0)
		{
			fprintf(stderr, "exec error\n");
			exit(1);
		}
	}

	if(wait3(&status, 0, &rusage) == pid)
		ssu_print_child_info(status, &rusage);
	else
	{
		fprintf(stderr, "wait3 error\n");
		exit(1);
	}

	exit(0);
}

double ssu_maketime(struct timeval *time)
{
	//시간을 마이크로초 단위로 구성
	return ((double)time->tv_sec+(double)time->tv_usec/1000000.0);
}

void term_stat(int stat)
{
	//정상종료시
	if (WIFEXITED(stat))
		printf("normally terminated. exit status = %d\n", WIFEXITED(stat));
	//비정상 종료시
	else if (WIFSIGNALED(stat))
		printf("abnormal termination byb signal %d. %s\n", WTERMSIG(stat), 
				//코어덤프
#ifdef WCOREDUMP
				WCOREDUMP(stat)?"core dumped":"no core"
#else
				NULL
#endif
			  );
	//시그널로 종료시
	else if (WIFSTOPPED(stat))
		printf("stopped by signal %d\n", WSTOPSIG(stat));
}

void ssu_print_child_info(int stat, struct rusage *rusage)
{
	//종료시 정보
	printf("Termination info follows\n");
	term_stat(stat);
	printf("user CPU time : %.2f(sec)\n", ssu_maketime(&rusage->ru_utime));
	printf("system CPU time : %.2f(sec)\n", 
			ssu_maketime(&rusage->ru_stime));
}
