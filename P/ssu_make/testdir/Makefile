CC = gcc
OS ?= LINUX

all : test

test : io.o util.o pp.o struct.o parser.o test.o
	$(CC) -W -Wall -o test io.o util.o pp.o struct.o parser.o test.o

io.o : io.c
	$(CC) -W -Wall -c -o io.o io.c
	
util.o : util.c
	$(CC) -W -Wall -c -o util.o util.c
	
	pp.o : pp.c
	$(CC) -W -Wall -c -o pp.o pp.c

struct.o : struct.c
	$(CC) -W -Wall -c -o struct.o struct.c

parser.o : parser.c
	$(CC) -W -Wall -c -o parser.o parser.c

test.o : test.c
	$(CC) -W -Wall -c -o test.o test.c

clean : 
	rm -rf *.o test 
