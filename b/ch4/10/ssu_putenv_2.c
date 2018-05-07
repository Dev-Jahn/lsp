#include <stdio.h>
#include <stdlib.h>

void ssu_printenv(char *label, char ***envpp);

extern char **environ;

int main(int argc, char *argv[], char *envp[])
{
	//변경전 환경변수목록 출력
	ssu_printenv("Initially", &envp);
	//환경변수 설정
	putenv("TZ=PST8PDT");
	//변경후 환경변수목록 출력
	ssu_printenv("After changing TZ", &envp);
	//환경변수 설정
	putenv("WARNING=Don't use envp after putenv()");
	//변경후 환경변수목록 출력
	ssu_printenv("After setting a new variable", &envp);
	//WARNING 변수내용 출력
	printf("value of WARNING is %s\n", getenv("WARNING"));
	exit(0);
}

void ssu_printenv(char *label, char ***envpp)
{
	char **ptr;

	printf("---- %s ---\n", label);
	printf("envp is at %8o and contains %8o\n", envpp, *envpp);
	printf("environ is at %8o and contains %8o\n", &environ, environ);
	printf("My environment variable are:\n");

	for (ptr = environ; *ptr; ptr++)
		printf("(%8o) = %8o -> %s\n", ptr, *ptr, *ptr);
	printf("(%8o) = %8o\n", ptr, *ptr);
}
