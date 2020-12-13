CC=gcc

source: source.c ansin.c alex.c
	$(CC) -o source source.c ansin.c alex.c

clean: 
	rm -f source