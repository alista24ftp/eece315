CFLAGS = -g -Wall
CC = gcc

all:	vmmanager

vmmanager: vmmanager.o translate.o update.o
	$(CC) -o vmmanager $(CFLAGS) vmmanager.o  translate.o update.o

clean: 
	rm -f *.o vmmanager 
