CC		= gcc
CFLAGS	= -W -Wall -g
OS		= LINUX 
#simple comment

ssu_make : io.o util.o patterns.o pp.o struct.o\
	parser.o ui.o core.o main.o
	$(CC) $(CFLAGS) -o $@ $^ 

io.o : io.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
util.o : util.c
	$(CC) $(CFLAGS) -c -o $@ $^

patterns.o : patterns.c
	$(CC) $(CFLAGS) -c -o $@ $^

pp.o : pp.c
	$(CC) $(CFLAGS) -c -o $@ $^

struct.o : struct.c
	$(CC) $(CFLAGS) -c -o $@ $^

parser.o : parser.c
	$(CC) $(CFLAGS) -c -o $@ $^

ui.o : ui.c
	$(CC) $(CFLAGS) -c -o $@ $^

core.o : core.c
	$(CC) $(CFLAGS) -c -o $@ $^

main.o : main.c
	$(CC) $(CFLAGS) -c -o $@ $^

done :
	rm -rf *.o
clean : 
	rm -rf *.o ssu_make

t1 : t2 t3
	echo t1

t2 :
	echo "My os is $(OS)" 

t3 :
	echo t3
