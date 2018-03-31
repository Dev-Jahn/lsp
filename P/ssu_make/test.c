#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "struct.h"
#include "util.h"
#include "patterns.h"
int main()
{

	int fd;
	fd = open("testdir/mk",O_RDONLY);
	Off_Pair o = regfind(fd, pat_target);
	printf("so:%ld\teo:%ld\n",o.so,o.eo);

}
