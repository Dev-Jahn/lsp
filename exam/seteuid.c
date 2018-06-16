#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

int main(void)
{
	/*int fd = open("uidtest", O_RDWR|O_CREAT, 0600);*/
	/*printf("fd:%d\n", fd);*/
	/*write(fd, "asdf", 4);*/
	/*lseek(fd, 0, SEEK_SET);*/

	/*close(fd);*/

	/*printf("setuid:%d\n",seteuid(getuid()));*/
	printf("euid:%d\n", geteuid());


	int fd = open("uidtest", O_RDONLY);
	printf("euid:%d\n", geteuid());
	printf("fd:%d\n", fd);
	printf("errno:%s\n", strerror(errno));


	char buf[100];
	int r =	read(fd, buf, 100);
	printf("read:%d\n", r);
	printf("%s\n", buf);
}
