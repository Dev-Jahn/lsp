#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#define SIZ 1024
int main(int argc, char *argv[])
{
	int opts = 0;
	int c;
	while ((c = getopt(argc,argv,"s"))!=-1)
	{
		switch(c)
		{
		case 's':
			opts = 1;
			break;
		}
	}
	if (argc-optind != 1)
	{
		fprintf(stderr, "arg wrong\n");
		exit(1);
	}
	char lname[SIZ];
	strcpy(lname, argv[optind]);
	strcat(lname,"_l");
	char sname[SIZ];
	strcpy(sname, argv[optind]);
	strcat(sname,"_s");
	
	printf("%d\n",opts);
	if (opts)
		symlink(argv[optind],sname);
	else
		link(argv[optind],lname);
}

