CC = gcc
CFLAGS = -Wall -std=c99

all: prog2tree sleepOrPause

prog2tree: prog2tree.o 
	$(CC) $(CFLAGS) -o prog2tree prog2tree.o

sleepOrPause: sleepOrPause.o
	$(CC) $(CFLAGS) -o sleepOrPause sleepOrPause.o

prog2tree.o: prog2tree.c prog2tree.h
	$(CC) $(CFLAGS) -c prog2tree.c

sleepOrPause.o: sleepOrPause.c sleepOrPause.h
	$(CC) $(CFLAGS) -c sleepOrPause.c

clean:
	rm *.o prog2tree sleepOrPause
