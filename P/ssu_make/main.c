#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "ssu_make.h"

#include <regex.h>

int main(int argc, char *argv[])
{
	char *pat = "\\w+=";
	regex_t regex;
	regcomp(&regex, pat, REG_EXTENDED);
	int ret = compare(pat, "NAME=JOHN");
	printf("compare: %d\n", ret);
	char *trimmed = trim(pat, "NAME=JOHN");
	trimmed[strlen(trimmed)-1] = 0;
	
	printf("%s\n", trimmed);

/*	for (int i=0;i<argc;i++)
	{
		printf("%s\n",argv[i]);
	}
	*/
	exit(0);
}
