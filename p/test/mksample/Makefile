CC		= gcc
CFLAGS	= -W -Wall -ggdb
#simple comment
all : mksample done

mksample : main.o 
	$(CC) $(CFLAGS) -o $@ $^ 

main.o : main.c
	$(CC) $(CFLAGS) -c -o $@ $^

done :
	rm -rf *.o

clean : 
	rm -rf *.o mksample
