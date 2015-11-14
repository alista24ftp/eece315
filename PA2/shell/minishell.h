#define LINE_LEN 80
#define MAX_ARGS 64
#define MAX_ARG_LEN 16
#define MAX_PATHS 64
#define MAX_PATH_LEN 96
#define WHITESPACE " ,\t\n"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h> // for getcwd() in printPrompt.c
#include <sys/types.h>
#include <sys/wait.h>

#ifndef NULL
#define NULL
#endif

struct command_t{
	char *name;
	int argc;
	char *argv[MAX_ARGS];
	
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
char *lookupPath(char **, char **);
int parsePath(char **);
void printPrompt();
void readCommand(char *);
int ampAtEndOfString(char *);
int redir(struct command_t *);