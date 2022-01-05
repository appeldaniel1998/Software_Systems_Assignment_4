CC = gcc
CFLAGS = -Wall
TARGETS = graph


all: $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $<


graph: main.o
	$(CC) $(CFLAGS) -o graph $^

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o *.a *.os
