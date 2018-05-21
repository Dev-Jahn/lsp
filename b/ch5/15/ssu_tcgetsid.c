#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <termios.h>

int main(void)
{
	pid_t sid_stderr;
	pid_t sid_stdin;
	pid_t sid_stdout;

	/*Get the group id of stdin control terminal's session leader*/
	sid_stdin = tcgetsid(STDIN_FILENO);
	if (sid_stdin ==-1)
	{
		fprintf(stderr, "tcgetsid error\n");
		exit(1);
	}
	else
		printf("Session leader for stdin: %d\n", sid_stdin);

	/*Get the group id of stdout control terminal's session leader*/
	sid_stdout = tcgetsid(STDOUT_FILENO);
	if (sid_stdout==-1)
	{
		fprintf(stderr, "tcgetsid error\n");
		exit(1);
	}
	else
		printf("Session leader for stdout: %d\n", sid_stdout);

	/*Get the group id of stderr control terminal's session leader*/
	sid_stderr= tcgetsid(STDERR_FILENO);
	if (sid_stderr==-1)
	{
		fprintf(stderr, "tcgetsid error\n");
		exit(1);
	}
	else
		printf("Session leader for stderr: %d\n", sid_stderr);

	exit(0);
}
