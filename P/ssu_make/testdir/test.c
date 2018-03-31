#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "util.h"
#include "struct.h"
#include "pp.h"
#include "io.h"
#include "patterns.h"
#include "parser.h"

int main()
{
	int fd = open("testdir/txt.tmp", O_RDWR);
	char buf[100];
	delLine(fd);
	delLine(fd);
	delLine(fd);
	

	/*preprocess("testdir/txt",NULL);*/
}
