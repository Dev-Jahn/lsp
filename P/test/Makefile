all : test

test : a.o b.o main.o
	gcc -W -Wall -o test a.o b.o main.o

a.o : a.c
	gcc -W -Wall -c -o a.o a.c

b.o : b.c
	gcc -W -Wall -c -o b.o b.c

main.o : main.c
	gcc -W -Wall -c -o main.o main.c

clean :
	rm -rf *.o test
