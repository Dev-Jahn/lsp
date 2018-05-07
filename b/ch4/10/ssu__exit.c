#include <stdio.h>
#include <unistd.h>
int main(void)
{
	//문자열 출력
	printf("Good afternoon?");
	//표준입출력 정리하지않고 프로세스 종료
	_exit(0);
}
