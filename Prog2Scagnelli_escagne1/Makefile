CC = gcc
CFLAGS = -Wall -std=c99

all: prog2tree

prog2tree: prog2tree.o 
	$(CC) $(CFLAGS) -o prog2tree prog2tree.o

prog2tree.o: prog2tree.c prog2tree.h
	$(CC) $(CFLAGS) -c prog2tree.c

clean:
	rm *.o prog2tree
