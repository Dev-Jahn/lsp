CC		= gcc
CFLAGS	= -W -Wall -ggdb
#simple comment
all : ssu_cp done

ssu_cp : copy.o error.o ssu_cp.o 
	$(CC) $(CFLAGS) -o $@ $^ 

copy.o : copy.c
	$(CC) $(CFLAGS) -c -o $@ $^

error.o : error.c
	$(CC) $(CFLAGS) -c -o $@ $^

main.o : main.c
	$(CC) $(CFLAGS) -c -o $@ $^

done :
	rm -rf *.o
clean : 
	rm -rf *.o ssu_cp 
