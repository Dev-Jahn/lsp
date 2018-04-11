#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include "ssu_cp.h"
#include "error.h"

int flag = 000;
int proc_num = 0;
int main(int argc, char *argv[])
{
	char src[PATH_MAX];
	char tgt[PATH_MAX];
	int argi = setopt(argc, argv);
	strcpy(src, argv[argi]);
	strcpy(tgt, argv[argi+1]);
	if (argv[argi+2] != NULL)
		error(USAGE,NULL);

	exit(0);
}

int setopt(int argc, char *argv[])
{
	int c;
	while ((c=getopt(argc, argv, "silnprd:g")) != -1)
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
				flag = flag|OPT_R;
				break;
			case 'd':
				if (ON_D(flag))
					error(REOPT, NULL);
				flag = flag|OPT_D;
				proc_num = atoi(optarg);
				break;
			case 'b':
				if (ON_B(flag))
					error(REOPT, NULL);
				flag = flag|OPT_B;
				break;
		}
		if ((flag&(~OPT_S))!=0000)
	}
	return optind;
}

