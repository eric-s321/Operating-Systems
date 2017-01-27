all: prog1sorter

prog1sorter: prog1sorter.o 
	gcc -Wall -std=c99 -o prog1sorter prog1sorter.o

prog1sorter.o: prog1sorter.c timing.c
	gcc -Wall -std=c99 -c prog1sorter.c timing.c

timing.o: timing.c
	gcc -Wall -std=c99 -c timing.c

clean:
	rm *.o prog1sorter 
