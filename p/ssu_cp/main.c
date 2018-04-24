#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include "main.h"
#include "copy.h"
#include "error.h"
#include "util.h"
//옵션 저장플래그
int flag = 000;
//d옵션 사용시 인자로 받을 프로세스 수
int proc_num = 0;
int main(int argc, char *argv[])
{
	struct timeval begin, end;
	char src[PATH_MAX];
	char tgt[PATH_MAX];
	//옵션설정
	int argi = setopt(argc, argv);
	//옵션출력
	printopt();
	strcpy(src, argv[argi]);
	strcpy(tgt, argv[argi+1]);
	printf("source : %s\n",src);
	printf("target : %s\n",tgt);

	if (ON_T(flag))
		gettimeofday(&begin, NULL);
	//인자 입력 틀리면 에러
	if (argv[argi+2] != NULL)
		error(USAGE,NULL);
	//문제없으면 src를 tgt로 복사
	else
		copy(src, tgt);
	//경과시간 계산후 출력
	if (ON_T(flag))
	{
		gettimeofday(&end, NULL);
		printf("Elapsed time: %ld us.\n", end.tv_usec-begin.tv_usec);
	}
	exit(0);
}

int setopt(int argc, char *argv[])
{
	int c;
	while ((c=getopt(argc, argv, "silnprd:bt")) != -1)
	{
		char option[3] = {'-', c, '\0'};
		switch(c)
		{
			case 's':
				if (ON_S(flag))
					error(REOPT, option);
				flag = flag|OPT_S;
				break;
			case 'i':
				if (ON_I(flag))
					error(REOPT, option);
				if (ON_N(flag))
					error(COLOPT,option);
				flag = flag|OPT_I;
				break;
			case 'l':
				if (ON_L(flag))
					error(REOPT, option);
				flag = flag|OPT_L;
				break;
			case 'n':
				if (ON_N(flag))
					error(REOPT, option);
				if (ON_I(flag))
					error(COLOPT, option);
				flag = flag|OPT_N;
				break;
			case 'p':
				if (ON_P(flag))
					error(REOPT, option);
				flag = flag|OPT_P;
				break;
			case 'r':
				if (ON_R(flag))
					error(REOPT, option);
				if (ON_D(flag))
					error(COLOPT, option);
				flag = flag|OPT_R;
				break;
			case 'd':
				if (ON_D(flag))
					error(REOPT, option);
				if (ON_R(flag))
					error(COLOPT, option);
				if (strcmp(optarg,"r") == 0)
				{
					option[1] = 'r';
					error(COLOPT, option);
				}
				flag = flag|OPT_D;
				if (compare("^(10|[1-9])$", optarg) == 0)
					proc_num = atoi(optarg);
				else
					error(ARGD, optarg);
				break;
			case 'b':
				if (ON_B(flag))
					error(REOPT, option);
				flag = flag|OPT_B;
				break;
			case 't':
				if (ON_T(flag))
					error(REOPT, option);
				flag = flag|OPT_T;
				break;
		}
		//S와 다른 플래그가 함께 켜져있으면
		if ((flag&(~OPT_S))!=0000 && ON_S(flag))
			error(SONLY, NULL);
	}
	return optind;
}

void printopt()
{
	if (ON_S(flag))
		printf("s option is on.\n");
	if (ON_I(flag))
		printf("i option is on.\n");
	if (ON_L(flag))
		printf("l option is on.\n");
	if (ON_N(flag))
		printf("n option is on.\n");
	if (ON_P(flag))
		printf("p option is on.\n");
	if (ON_R(flag))
		printf("r option is on.\n");
	if (ON_D(flag))
		printf("d option is on.\n");
	if (ON_B(flag))
		printf("b option is on.\n");
	if (ON_T(flag))
		printf("t option is on.\n");
}
