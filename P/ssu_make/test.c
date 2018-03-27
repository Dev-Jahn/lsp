#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "util.h"
#include "struct.h"
#include "pp.h"
#include "io.h"
#include "patterns.h"

int main()
{
	int fd = open("testdir/txt", O_RDWR);
	/*int fd2 = open("testdir/txt.tmp", O_RDWR);*/
	char buf[100];
	Off_Pair p;
	char *a = "test_make1 : test_code1o";
	char *b = "test_make1 : test_code1.o 	   	 ";

	char *c = "\tgcc -o test_make1 test_code1.o";
	char *d = "\t  gcc -c test_code1.c";
	char *e = "\tclear";
	int i = compare(pat_cmd, c);
	int j = compare(pat_cmd, d);
	int k = compare(pat_cmd, e);
	printf("%d\n",i);
	printf("%d\n",j);
	printf("%d\n",k);
	
	/*
	 *p = regfind(fd, "\\\\\n");
	 *printf("so:%ld\teo:%ld\n",p.so,p.eo);
	 *read(fd, buf, 20);
	 *printf("%s\n",buf);
	 */
	/*freplace(fd2, "\\\\\n", "");*/
	/*preprocess("testdir/txt",NULL);*/
}
