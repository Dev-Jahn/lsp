#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "util.h"
#include "struct.h"
int main ()
{
	int fd = open("include",O_RDONLY);
	Off_Pair off;
	do {
		off = regfind(fd, "include");
		printf("start:%ld\nend:%ld\nfound:%d\n\n",off.so,off.eo,off.found);
	} while(off.found);
}

