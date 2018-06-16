#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
	int sigkill = 0;
	struct option opts[] = 
	{
		{"9", no_argument, NULL, SIGKILL},
		{"SIGKILL", no_argument, NULL, SIGKILL}
	};
	int opt, longind;

	while ((opt = getopt_long_only(argc, argv, "", opts, &longind)) != -1)
	{
		if (argc!=3)
		{
			printf("Invalid arguments1\n");
			exit(1);
		}

		switch (opt)
		{
		case SIGKILL:
			sigkill = 1;
			break;
		case '?':
			printf("Unknown option\n");
			exit(1);
		}
	}

	if (!sigkill && argc!=2)
	{
		printf("Invalid arguments2\n");
		exit(1);
	}

	if (sigkill)
		kill(atoi(argv[optind]), SIGKILL);
	else
		kill(atoi(argv[optind]), SIGTERM);
}
