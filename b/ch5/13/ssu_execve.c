#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	//명령행 인자벡터
	char *argv[] = {
		"ssu_execl_test_1", "param1", "param2", (char *)0
	};
	//환경변수벡터
	char *env[] = {
		"NAME = value", 
		"nextname=nextvalue", 
		"HOME=/home/jahn", (char *)0 };
	printf("this is the original program\n");
	//인자와 환경변수로 프로그램 실행
	execve("./ssu_execl_test_1", argv, env);
	//정상실행시 이후는 실행되지 않음
	printf("%s\n", "This line sould nver get printed\n");
	exit(0);
}
