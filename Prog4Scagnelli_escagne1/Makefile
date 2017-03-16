CC = g++
CPPFLAGS = -Wall

all: prog4

prog4: prog4.o
	$(CC) $(CPPFLAGS) -o prog4 prog4.o

prog4.o: prog4.cpp prog4.h
	$(CC) $(CFLAGS) -c prog4.cpp

clean:
	rm *.o prog4
