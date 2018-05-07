#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void ssu_addone(void);
//환경변수목록을 가리키는 전역변수
extern char **environ;
//환경변수로 설정할 문자열
char glob_var[] = "HOBBY=swimming";

int main(void)
{
	int i;
	//환경변수 목록 출력
	for (i=0;environ[i] != NULL;i++)
		printf("environ[%d] : %s\n", i, environ[i]);
	//전역변수로 환경변수 추가
	putenv(glob_var);
	//지역변수로 환경변수 추가
	ssu_addone();
	//설정한 환경변수 출력
	printf("My hobby is %s\n", getenv("HOBBY"));
	printf("My lover is %s\n", getenv("LOVER"));
	//전역변수 내용 변경
	strcpy(glob_var+6, "fishing");
	//환경변수 목록 출력
	for(i=0;environ[i]!=NULL;i++)
		printf("environ[%d] : %s\n", i, environ[i]);

	exit(0);
}

void ssu_addone(void)
{
	//지역변수 배열
	char auto_var[10];
	//내용복사
	strcpy(auto_var, "LOVER=js");
	//지역변수로 환경변수 설정
	putenv(auto_var);
}
