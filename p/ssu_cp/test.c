#include <stdio.h>
#include <string.h>
#include "copy.h"
#include "util.h"
char path[100];
int flag = 0000;
int main(void)
{
	printf("10:%d\n", compare("^(10|[1-9])$", "10"));
	printf("0:%d\n", compare("^(10|[1-9])$", "0"));
	printf("1:%d\n", compare("^(10|[1-9])$", "1"));
	printf("11:%d\n", compare("^(10|[1-9])$", "11"));
	printf("30:%d\n", compare("^(10|[1-9])$", "30"));
	printf("99:%d\n", compare("^(10|[1-9])$", "99"));
	printf("9:%d\n", compare("^(10|[1-9])$", "9"));
}
