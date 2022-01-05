all: graph

graph: main.o
	gcc -Wall -g -o graph main.o

main.o: main.c
	gcc -Wall -g -c main.c

.PHONY: clean all

clean:
	rm -f *.o *.a graph
