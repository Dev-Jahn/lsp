#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("abort terminate this program\n");
	/*Raise SIGABRT*/
	abort();
	printf("this line is never reached\n");
	exit(0);
}
