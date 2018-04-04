#include <stdio.h>
#include <unistd.h>
#include "ssu_cp.h"

int flag = 000;
int main(int argc, char *argv[])
{
	exit(0);
}

int setopt(int argc, char *argv[])
{
	int c;
	while ((c=getopt(argc, argv, "mhstpDf:c:")) != -1)
	{
		switch(c)
		{
			case 's':
				flag = flag|OPT_S;
				break;
			case 'i':
				flag = flag|OPT_I;
				break;
			case 'l':
				flag = flag|OPT_L;
				break;
			case 'n':
				flag = flag|OPT_N;
				break;
			case 'p':
				flag = flag|OPT_P;
				break;
			case 'r':
				flag = flag|OPT_R;
				break;
			case 'd':
				flag = flag|OPT_D;
				break;
			case 'g':
				flag = flag|OPT_G;
				break;
		}
	}
	return optind;
}

