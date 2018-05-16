
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	/*system실행전 출력*/
	printf("before system()\n");
	/*시스템 명령 pwd 실행*/
	system("pwd");
	/*system실행후 출력*/
	printf("after system()\n");
	exit(0);
}
