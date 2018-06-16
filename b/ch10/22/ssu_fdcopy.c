#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

#define MSG "message will be written to terminal\n"

int main(void)
{
	int new_fd;
	/*copy descriptor 1 to 3*/
	if ((new_fd = fcntl(STDOUT_FILENO, F_DUPFD, 3))==-1)
	{
		fprintf(stderr, "Error : Copying File Descriptor\n");
		exit(1);
	}
	/*close 1*/
	close(STDOUT_FILENO);
	/*write string to 3*/
	write(3, MSG, strlen(MSG));
	exit(0);
}
