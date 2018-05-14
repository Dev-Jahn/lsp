NAME	= ssu_backup
CC		= gcc
CFLAGS	= -W -Wall -ggdb
LIBS	= -lssl -lcrypto -lpthread
all : $(NAME) done

$(NAME) : ssu_backup.o daemon.o io.o logger.o util.o error.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

ssu_backup.o : ssu_backup.c
	$(CC) $(CFLAGS) -c -o $@ $^

daemon.o : daemon.c
	$(CC) $(CFLAGS) -c -o $@ $^

io.o : io.c
	$(CC) $(CFLAGS) -c -o $@ $^

logger.o : logger.c
	$(CC) $(CFLAGS) -c -o $@ $^

util.o : util.c
	$(CC) $(CFLAGS) -c -o $@ $^

error.o : error.c
	$(CC) $(CFLAGS) -c -o $@ $^

test : test.o util.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

test.o : test.c
	$(CC) $(CFLAGS) -c -o $@ $^

done :
	rm -rf *.o

clean : 
	rm -rf *.o $(NAME)
