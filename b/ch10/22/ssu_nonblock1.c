#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

void set_flags(int fd, int flags);
void clr_flags(int fd, int flags);

char buf[500000];

int main(void)
{
	int ntowrite, nwrite;
	char *ptr;
	/*get input from stdin*/
	ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
	/*print number of bytes*/
	fprintf(stderr, "reading %d bytes\n", ntowrite);
	/*set stdout as nonblocking io*/
	set_flags(STDOUT_FILENO, O_NONBLOCK);
	ptr=buf;
	/*write all the buffer*/
	while(ntowrite>0)
	{
		errno=0;
		/*write to stdout*/
		nwrite = write(STDOUT_FILENO, ptr, ntowrite);
		fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);
		if (nwrite>0)
		{
			ptr+=nwrite;
			ntowrite-=nwrite;
		}
	}
	/*remove nonblock flag from descriptor*/
	clr_flags(STDOUT_FILENO, O_NONBLOCK);
	exit(0);
}

void set_flags(int fd, int flags)
{
	int val;
	/*get file flag*/
	if ((val = fcntl(fd, F_GETFL, 0))<0)
	{
		fprintf(stderr, "fcntl F_GETFL failed");
		exit(1);
	}
	/*add flags to current flag*/
	val|=flags;
	/*set flag to file*/
	if (fcntl(fd, F_SETFL, val)<0)
	{
		fprintf(stderr, "fcntl F_SETFL failed");
		exit(1);
	}
}

void clr_flags(int fd, int flags)
{
	int val;
	/*get file flag*/
	if ((val = fcntl(fd, F_GETFL, 0))<0)
	{
		fprintf(stderr, "fcntl F_GETFL failed");
		exit(1);
	}

	/*exclude flags from current flag*/
	val&=~flags;
	/*set flag to file*/
	if(fcntl(fd, F_SETFL, val)<0)
	{
		fprintf(stderr, "fcntl F_SETFL failed");
		exit(1);
	}
}
