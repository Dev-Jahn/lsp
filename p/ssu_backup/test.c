#include <stdio.h>
#include "util.h"

int main(void)
{
	int cnt;
	int pids[32768];
	cnt = findpid("tail", pids, 32768);
	for (int i=0;i<cnt;i++)
		printf("pid:%d\n", pids[i]);

}
