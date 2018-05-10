NAME	= ssu_backup
CC		= gcc
CFLAGS	= -W -Wall -ggdb
all : $(NAME) done

$(NAME) : ssu_backup.o daemon.o copy.o logger.o util.o error.o
	$(CC) $(CFLAGS) -o $(NAME) $^ 

ssu_backup.o : ssu_backup.c
	$(CC) $(CFLAGS) -c -o $@ $^

daemon.o : daemon.c
	$(CC) $(CFLAGS) -c -o $@ $^

copy.o : copy.c
	$(CC) $(CFLAGS) -c -o $@ $^

logger.o : logger.c
	$(CC) $(CFLAGS) -c -o $@ $^

util.o : util.c
	$(CC) $(CFLAGS) -c -o $@ $^

error.o : error.c
	$(CC) $(CFLAGS) -c -o $@ $^

test : test.o daemon.o logger.o util.o error.o
	$(CC) $(CFLAGS) -o $@ $^ 

test.o : test.c
	$(CC) $(CFLAGS) -c -o $@ $^

done :
	rm -rf *.o

clean : 
	rm -rf *.o $(NAME)
