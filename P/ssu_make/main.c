#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "ssu_make.h"

int opt_m, opt_h, opt_s, opt_t, opt_f, opt_c;

int main(int argc, char *argv[])
{
	opt_m = opt_h = opt_s = opt_t = opt_f = opt_c = 0;
	COMMAND *cmd = parse_cmd(argc, argv);

	exit(0);
}
