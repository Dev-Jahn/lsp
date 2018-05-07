#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	int i;
	//프로그램 실행인자 전체 출력	
	for (i=0;i<argc;i++)
		printf("argv[%d] : %s\n", i, argv[i]);
	//종료
	exit(0);
}
