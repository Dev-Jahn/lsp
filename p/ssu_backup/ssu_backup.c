#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include "ssu_backup.h"
#include "error.h"

//옵션 저장플래그
int flag = 000;

int main(int argc, char *argv[])
{
	opterr = 0;
	int argi = setopt(argc, argv);
}

int setopt(int argc, char *argv[])
{
	int c;
	while ((c=getopt(argc, argv, "drmcn:")) != -1)
	{
		switch(c)
		{
		case 'd':
			flag = flag|OPT_D;
			break;
		case 'r':
			flag = flag|OPT_R;
			break;
		case 'm':
			flag = flag|OPT_M;
			break;
		case 'c':
			flag = flag|OPT_C;
			break;
		case 'n':
			flag = flag|OPT_N;
			break;
		case '?':
			char charr[2] = { optopt, 0};
			break;

	}
		//S와 다른 플래그가 함께 켜져있으면
		/*if ((flag&(~OPT_S))!=0000 && ON_S(flag))*/
			/*error(SONLY, NULL);*/
	}
	return optind;
}
