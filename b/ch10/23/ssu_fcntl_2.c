#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssu_employee.h"

#define DUMMY 0

int main(int argc, char *argv[])
{
	struct ssu_employee record;
	int fd;
	int flags;
	int length;
	int pid;

	if (argc<2)
	{
		fprintf(stderr, "Usage : %s file\n", argv[0]);
		exit(1);
	}
	/*open as RDWR*/
	if ((fd = open(argv[1], O_RDWR))<0)
	{
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}
	
	/*get file flag*/
	if ((flags = fcntl(fd, F_GETFL, DUMMY)) == -1)
	{
		fprintf(stderr, "fcntl F_GETFL error\n");
		exit(1);
	}
	/*add O_APPEND	*/
	flags|=O_APPEND;
	/*set file flag*/
	if ((flags = fcntl(fd, F_SETFL, flags)) == -1)
	{
		fprintf(stderr, "fcntl F_SETFL error\n");
		exit(1);
	}

	pid = getpid();

	while (1)
	{
		/*get name from stdin*/
		printf("Enter employee name : ");
		scanf("%s", record.name);
		/*end writing		*/
		if (record.name[0] == '.')
			break;
		/*get salary from stdin*/
		printf("Enter employee salary : ");
		scanf("%d", &record.salary);
		/*put pid*/
		record.pid = pid;
		length = sizeof(record);
		/*append to file*/
		if (write(fd, (char *)&record, length) != length)
		{
			fprintf(stderr, "record write error\n");
			exit(1);
		}
	}
	close(fd);
	exit(0);
}
