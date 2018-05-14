#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	//param1 param2 인자로 ~test_1 프로그램 실행
	char *argv[] = {
		"ssu_execl_test_1", "param1", "param2", (char *)0
	};
	//문자열출력
	printf("this is the original program\n");
	execv("./ssu_execl_test_1", argv);
	//정상 실행시 이후는 실행되지않음
	printf("%s\n", "This line should never get printed\n");
	exit(0);
}
