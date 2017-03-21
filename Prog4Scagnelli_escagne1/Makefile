CC = g++
CPPFLAGS = -Wall -std=c++11

all: prog4pages

prog4pages: prog4pages.o
	$(CC) $(CPPFLAGS) -o prog4pages prog4pages.o

prog4pages.o: prog4pages.cpp prog4pages.hpp
	$(CC) $(CPPFLAGS) -c prog4pages.cpp

clean:
	rm *.o prog4pages
