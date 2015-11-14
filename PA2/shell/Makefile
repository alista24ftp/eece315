CFLAGS = -g -Wall
CC=gcc

all:		launch practiceParse

launch:	launch.o printPrompt.o lookupPath.o parsePath.o readCommand.o parseCommand.o  
	$(CC) -o launch $(CFLAGS) launch.o printPrompt.o lookupPath.o parsePath.o readCommand.o parseCommand.o  

practiceParse:	practiceParse.o 
	$(CC) -o practiceParse $(CFLAGS) practiceParse.o 

clean:
	rm -f *.o launch practiceParse