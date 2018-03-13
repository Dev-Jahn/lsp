all : ssu_make

ssu_make : struct.o parser.o main.o
	gcc -W -Wall -o ssu_make struct.o parser.o main.o

struct.o : struct.c
	gcc -W -Wall -c -o struct.o struct.c

parser.o : parser.c
	gcc -W -Wall -c -o parser.o parser.c

main.o : main.c
	gcc -W -Wall -c -o main.o main.c

clean : 
	rm -rf *.o ssu_make
