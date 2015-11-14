CFLAGS = -g -lrt -Wall
CC = gcc

all: sor

sor: sor.o
	$(CC) -o sor $(CFLAGS) sor.o 

clean: 
	rm -f *.o sor