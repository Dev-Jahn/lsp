#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

#define ssu_show(limit) ssu_show_limit(limit, #limit)

void ssu_show_limit(int rlim_type, char *rlim_name);

int main(void)
{
	printf("	resource soft limit hard limit	\n\n");
	//CPU 시간
	ssu_show(RLIMIT_CPU);
	//최대파일크기
	ssu_show(RLIMIT_FSIZE);
	//최대데이터 크기
	ssu_show(RLIMIT_DATA);
	//최대스택크기
	ssu_show(RLIMIT_STACK);
	//최대코어파일크기
	ssu_show(RLIMIT_CORE);
	//최대거주집합크기
#ifdef RLIMIT_RSS
	ssu_show(RLIMIT_RSS);
#endif
	//최대잠긴기억영역 주소공간
#ifdef RLIMIT_MEMLOCK
	ssu_show(RLIMIT_MEMLOCK);
#endif
	//최대프로세스수
#ifdef RLIMIT_NPROC
	ssu_show(RLIMIT_NPROC);
#endif
	//NOFILE의 BSD 매크로
#ifdef RLIMIT_OFILE
	ssu_show(RLIMIT_OFILE);
#endif
	//최대열수있는 파일 수
#ifdef RLIMIT_NOFILE
	ssu_show(RLIMIT_NOFILE);
#endif
	//BSD에서 가상프로세스 메모리맵핑사이즈
#ifdef RLIMIT_VMEN
	ssu_show(RLIMIT_VMEN);
#endif
	
	exit(0);
}

void ssu_show_limit(int rlim_type, char *rlim_name)
{
	struct rlimit rlim;
	char cur[11], max[11];

	getrlimit(rlim_type, &rlim);

	if (rlim.rlim_cur == RLIM_INFINITY)
		strcpy(cur, "infinity");
	else
		sprintf(cur, "%10ld", rlim.rlim_cur);

	if (rlim.rlim_max == RLIM_INFINITY)
		strcpy(max, "infinity");
	else
		sprintf(max, "%10ld", rlim.rlim_max);

	printf("%15s : %10s %10s\n", rlim_name, cur, max);
}
