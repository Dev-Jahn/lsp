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
#include "parser.h"
#include <string.h>
int main()
{
	char *pat = "^\\w+\\s*(\\?)?=\\s*([[:graph:]]+\\s*)+$";
	
	char *a = "CC		= gcc";
	char *b = "CFLAGS	= -W -Wall -g";
	char *c = "all : ssu_make done";
	char *d = "	gcc -W -Wall -g -o $@ $^";
	char *e = "io.o : io.c";
	printf("%d\n",compare(pat_macro,a));
	printf("%d\n",compare(pat_macro,b));
	printf("%d\n",compare(pat_macro,c));
	printf("%d\n",compare(pat_macro,d));
	printf("%d\n",compare(pat_macro,e));
}
