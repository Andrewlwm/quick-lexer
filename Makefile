CC=gcc
CFLAGS=-Wall -o

source: source.c ansin.c alex.c
	$(CC) $(CFLAGS) source source.c ansin.c alex.c

clean: 
	rm -f source