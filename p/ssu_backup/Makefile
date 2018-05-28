.PHONY : done clean
.SUFFIXES : .c .o

NAME	= ssu_backup
OBJECTS	= ssu_backup.o daemon.o io.o data.o logger.o util.o gc.o
SRCS	= ssu_backup.c daemon.c io.c data.c logger.c util.c gc.c
CC		= gcc
CFLAGS	= -W -Wall -ggdb -c
LIBS	= -lpthread
LIBS2	= -lssl -lcrypto -lpthread
SHA 	:= $(shell if [ -f "/usr/include/openssl/sha.h" ];then echo 1;else echo 0;fi)

ifeq ($(SHA),1)
MACRO	= -DSHA1
else
MACRO 	=
endif

all : $(NAME) done

$(NAME) : $(OBJECTS)
	$(CC) -o $(NAME) $^ $(LIBS)

.c.o :
	$(CC) $(CFLAGS) $< $(MACRO)

ssu_backup.o : ssu_backup.c
daemon.o : daemon.c
io.o : io.c
data.o : data.c
logger.o : logger.c
util.o : util.c
gc.o : gc.c dummy
dummy :
	
done :
	rm -rf *.o

clean : 
	rm -rf *.o $(NAME)
