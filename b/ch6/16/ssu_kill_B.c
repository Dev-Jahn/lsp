#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	/*Wait for signal*/
	while(1)
	{
		printf("\n[OSLAB]");
		sleep(5);
	}
	exit(0);
}
