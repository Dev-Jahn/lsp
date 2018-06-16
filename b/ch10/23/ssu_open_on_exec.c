#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
int main(void)
{
	int fd;
	int val;
	//create file
	fd = open("exec_copy.txt", O_CREAT);
	//execute blank loop
	execl("./loop", "./loop", NULL);
	exit(0);
}
