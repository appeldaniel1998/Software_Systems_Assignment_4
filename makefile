CC = gcc
Flags = -Wall -g

all: graph

graph: main.o
	$(CC) $(Flags) -o graph main.o

main.o: main.c
	$(CC) $(Flags) -c main.c

.Phony:clean all
clean:
	rm -f *.o *.a graph
