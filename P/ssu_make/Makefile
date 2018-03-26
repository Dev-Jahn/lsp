CC = gcc
OS ?= LINUX

all : ssu_make

ssu_make : io.o util.o pp.o struct.o parser.o main.o
	$(CC) -W -Wall -o ssu_make io.o util.o pp.o struct.o parser.o main.o

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

main.o : main.c
	$(CC) -W -Wall -c -o main.o main.c

clean : 
	rm -rf *.o ssu_make
