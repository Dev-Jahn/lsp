NAME	= ssu_backup
CC		= gcc
CFLAGS	= -W -Wall -ggdb
LIBS	= -lpthread
LIBS2	= -lssl -lcrypto -lpthread
#MACRO	= -DSHA1
all : $(NAME) done

$(NAME) : ssu_backup.o daemon.o io.o data.o logger.o util.o error.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

ssu_backup.o : ssu_backup.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(MACRO)

daemon.o : daemon.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(MACRO)

io.o : io.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(MACRO)

data.o : data.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(MACRO)

logger.o : logger.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(MACRO)

util.o : util.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(MACRO)

error.o : error.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(MACRO)

test : test.o util.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

test.o : test.c
	$(CC) $(CFLAGS) -c -o $@ $^

done :
	rm -rf *.o

clean : 
	rm -rf *.o $(NAME)
