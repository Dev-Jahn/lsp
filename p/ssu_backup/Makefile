NAME	= ssu_backup
CC		= gcc
CFLAGS	= -W -Wall -ggdb
all : $(NAME) done

$(NAME) : $(NAME).o
	$(CC) $(CFLAGS) -o $@ $^ 

$(NAME).o : $(NAME).c
	$(CC) $(CFLAGS) -c -o $@ $^

test : test.o
	$(CC) $(CFLAGS) -o $@ $^ 

test.o : test.c
	$(CC) $(CFLAGS) -c -o $@ $^

done :
	rm -rf *.o

clean : 
	rm -rf *.o $(NAME)
