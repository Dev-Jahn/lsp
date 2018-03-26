#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "util.h"
#include "io.h"
int main()
{
	int fd = open("txt", O_RDWR);
	char buf[100];
	int num[10];
	Off_Pair o;


	freplace(fd, "\\$\\(CC\\)", "gcc");
	close(fd);
}
