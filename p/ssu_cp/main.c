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

int flag = 000;
int proc_num = 0;
int main(int argc, char *argv[])
{
	struct timeval begin, end;
	char src[PATH_MAX];
	char tgt[PATH_MAX];
	int argi = setopt(argc, argv);
	strcpy(src, argv[argi]);
	strcpy(tgt, argv[argi+1]);
	if (ON_T(flag))
		gettimeofday(&begin, NULL);
	if (argv[argi+2] != NULL)
		error(USAGE,NULL);
	else
		copy(src, tgt);
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
		switch(c)
		{
			case 's':
				if (ON_S(flag))
					error(REOPT, NULL);
				flag = flag|OPT_S;
				break;
			case 'i':
				if (ON_I(flag))
					error(REOPT, NULL);
				if (ON_N(flag))
					error(COLOPT,NULL);
				flag = flag|OPT_I;
				break;
			case 'l':
				if (ON_L(flag))
					error(REOPT, NULL);
				flag = flag|OPT_L;
				break;
			case 'n':
				if (ON_N(flag))
					error(REOPT, NULL);
				if (ON_I(flag))
					error(COLOPT, NULL);
				flag = flag|OPT_N;
				break;
			case 'p':
				if (ON_P(flag))
					error(REOPT, NULL);
				flag = flag|OPT_P;
				break;
			case 'r':
				if (ON_R(flag))
					error(REOPT, NULL);
				if (ON_D(flag))
					error(COLOPT, NULL);
				flag = flag|OPT_R;
				break;
			case 'd':
				if (ON_D(flag))
					error(REOPT, NULL);
				if (ON_R(flag))
					error(COLOPT, NULL);
				flag = flag|OPT_D;
				if (compare("^(10|[1-9])$", optarg) == 0)
					proc_num = atoi(optarg);
				else
					error(ARGD, optarg);
				break;
			case 'b':
				if (ON_B(flag))
					error(REOPT, NULL);
				flag = flag|OPT_B;
				break;
			case 't':
				if (ON_T(flag))
					error(REOPT, NULL);
				flag = flag|OPT_T;
				break;
		}
		//S와 다른 플래그가 함께 켜져있으면
		if ((flag&(~OPT_S))!=0000 && ON_S(flag))
			error(SONLY, NULL);
	}
	return optind;
}

