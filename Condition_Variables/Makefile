CC = gcc
CFLAGS = -Wall -std=c99 -pthread

all: fig1 fig2 fig3 fig3_poor1 fig3_poor2 pc_v1 fig6_broken fig8_broken2

fig1: fig1.o
	$(CC) $(CFLAGS) -o fig1 fig1.o

fig1.o: fig1.c
	$(CC) $(CFLAGS) -c fig1.c

fig2: fig2.o
	$(CC) $(CFLAGS) -o fig2 fig2.o

fig2.o: fig2.c
	$(CC) $(CFLAGS) -c fig2.c 

fig3: fig3.o
	$(CC) $(CFLAGS) -o fig3 fig3.o

fig3.o: fig3.c
	$(CC) $(CFLAGS) -c fig3.c

fig3_poor1: fig3_poor1.o
	$(CC) $(CFLAGS) -o fig3_poor1 fig3_poor1.o

fig3_poor1.o: fig3_poor1.c
	$(CC) $(CFLAGS) -c fig3_poor1.c

fig3_poor2: fig3_poor2.o
	$(CC) $(CFLAGS) -o fig3_poor2 fig3_poor2.o

fig3_poor2.o: fig3_poor2.c
	$(CC) $(CFLAGS) -c fig3_poor2.c

pc_v1: pc_v1.o
	$(CC) $(CFLAGS) -o pc_v1 pc_v1.c

pc_v1.o: pc_v1.c
	$(CC) $(CFLAGS) -c pc_v1.c

fig6_broken: fig6_broken.o
	$(CC) $(CFLAGS) -o fig6_broken fig6_broken.o

fig6_broken.o: fig6_broken.c
	$(CC) $(CFLAGS) -c fig6_broken.c

fig8_broken2: fig8_broken2.o
	$(CC) $(CFLAGS) -o fig8_broken2 fig8_broken2.c

fig8_broken2.o: fig8_broken2.c
	$(CC) $(CFLAGS) -c fig8_broken2.c

clean:
	rm *.o fig1 fig2 fig3 fig3_poor1 fig3_poor2 pc_v1 fig6_broken fig8_broken2



