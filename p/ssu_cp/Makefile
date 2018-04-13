CC		= gcc
CFLAGS	= -W -Wall -ggdb
#simple comment
all : ssu_cp done

ssu_cp : copy.o error.o util.o main.o 
	$(CC) $(CFLAGS) -o $@ $^ 

copy.o : copy.c
	$(CC) $(CFLAGS) -c -o $@ $^

error.o : error.c
	$(CC) $(CFLAGS) -c -o $@ $^

util.o : util.c
	$(CC) $(CFLAGS) -c -o $@ $^

main.o : main.c
	$(CC) $(CFLAGS) -c -o $@ $^

test : copy.o error.o test.o
	$(CC) $(CFLAGS) -o $@ $^ 

test.o : test.c
	$(CC) $(CFLAGS) -c -o $@ $^

done :
	rm -rf *.o

clean : 
	rm -rf *.o ssu_cp 
