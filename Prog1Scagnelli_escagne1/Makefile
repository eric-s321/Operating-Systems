CC = gcc
CFLAGS = -Wall -std=c99

all: prog1sorter prog1generator

prog1sorter: prog1sorter.o
	$(CC) $(CFLAGS) -o prog1sorter prog1sorter.o

prog1generator: prog1generator.o
	$(CC) $(CFLAGS) -o prog1generator prog1generator.o

prog1generator.o: prog1generator.c
	$(CC) $(CFLAGS) -c prog1generator.c

prog1sorter.o: prog1sorter.c
	$(CC) $(CFLAGS) -c prog1sorter.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

timing.o: timing.c
	$(CC) $(CFLAGS) -c timing.c

clean:
	rm *.o prog1sorter prog1generator
