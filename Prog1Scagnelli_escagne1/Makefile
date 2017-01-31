CC = gcc
CFLAGS = -Wall -std=c99

all: prog1sorter prog1generator

prog1sorter: prog1sorter.o parser.o timing.o
	$(CC) $(CFLAGS) -o prog1sorter prog1sorter.o parser.o timing.o

prog1generator: prog1generator.o parser.o timing.o
	$(CC) $(CFLAGS) -o prog1generator prog1generator.o parser.o timing.o

prog1generator.o: prog1generator.c prog1generator.h timing.h parser.h
	$(CC) $(CFLAGS) -c prog1generator.c

prog1sorter.o: prog1sorter.c prog1sorter.h timing.h parser.h
	$(CC) $(CFLAGS) -c prog1sorter.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

timing.o: timing.c timing.h
	$(CC) $(CFLAGS) -c timing.c

clean:
	rm *.o prog1sorter prog1generator
