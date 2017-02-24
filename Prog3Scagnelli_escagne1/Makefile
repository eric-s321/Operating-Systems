CC = gcc
CFLAGS = -Wall

all: prog3ipc

prog3ipc: prog3ipc.o
	$(CC) $(CFLAGS) -o prog3ipc prog3ipc.o -lrt

prog3ipc.o: prog3ipc.c prog3ipc.h
	$(CC) $(CFLAGS) -c prog3ipc.c

clean:
	rm *.o prog3ipc

