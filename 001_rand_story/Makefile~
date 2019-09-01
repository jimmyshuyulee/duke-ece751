CFLAGS=-std=gnu99 -pedantic -Wall -Werror -ggdb3
all: main.o
	gcc -o rand_story main.o
main.o: main.c catarray.h
	gcc -c $(CFLAGS) main.c

.PHONY: clean
clean:
	rm -r rand_story *.o *~
