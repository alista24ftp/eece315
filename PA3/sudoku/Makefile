CFLAGS = -g -pthread -Wall
CC=gcc

all:		sudoku_val

sudoku_val:	sudoku_val.o 
	$(CC) -o sudoku_val $(CFLAGS) sudoku_val.o 
	
clean:
	rm -f *.o sudoku_val