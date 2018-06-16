#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(void)
{
	int testfd;
	int fd;

	/*open file*/
	fd  = open("test.txt", O_CREAT);
	/*copy file descriptor 3 to 10*/
	testfd = fcntl(fd, F_DUPFD, 10);
	printf("testfd : %d\n", testfd);
	/*copy file descriptor 10 to 11*/
	testfd = fcntl(fd, F_DUPFD, 10);
	printf("testfd : %d\n", testfd);

	getchar();
}
