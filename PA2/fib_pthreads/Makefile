CFLAGS = -g -pthread -Wall
CC=gcc

all:		fib_pthreads

fib_pthreads:	fib_pthreads.o 
	$(CC) -o fib_pthreads $(CFLAGS) fib_pthreads.o 
	
clean:
	rm -f *.o fib_pthreads