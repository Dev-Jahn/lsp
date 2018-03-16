#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssu_make.h"

int main(int argc, char *argv[])
{
	for (int i=0;i<argc;i++)
	{
		printf("%s\n",argv[i]);
	}
	parse_cmd(argc, argv);
	exit(0);
}
