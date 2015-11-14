#include <string.h>

/**
 * Determine command name and construct the parameter list. This function will build
 * argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line and argv[] is an array of
 * strings (pointers to char *).
 * The last element in argv[] must be NULL. As we scan the command line from the left,
 * the first token goes in argv[0], the second in argv[l], and so on. Each time we add a
 * token to argv[], we increment argc.
 * 
 * PURPOSE: parse command line, and return result in the struct command_t *cmd argument
 */

#include "minishell.h"
 
int parseCommand(char *cLine, struct command_t *cmd) {
	int argc;
	char **clPtr;
	int hasAmpersand = 0;
	
	/* Initialization */
	clPtr = &cLine; /* cLine is the command line */
	argc = 0;
			
	/* Fill argv[] */
	cmd->argv[argc] = strtok(*clPtr, WHITESPACE);
	while(cmd->argv[argc] != NULL){
		argc++;
		cmd->argv[argc] = strtok(NULL, WHITESPACE);
	}
		
	/* Set the command name and argc */
	cmd->argc = argc - 1;
	cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
	strcpy(cmd->name, cmd->argv[0]);
	
	/******** FEATURE 2 - CHECK FOR AMPERSAND AT END OF COMMAND *********/
	if(!strcmp(cmd->argv[cmd->argc], "&")){
		cmd->argv[cmd->argc] = NULL;
		cmd->argc--;
		hasAmpersand = 1;
	}
	else if(ampAtEndOfString(cmd->argv[cmd->argc])){
		cmd->argv[cmd->argc] = strtok(cmd->argv[cmd->argc], "&");
		hasAmpersand = 1;
	}
	
	return hasAmpersand;
}

int ampAtEndOfString(char *str){
	if(str[strlen(str)-1] == '&') return 1;
	else return 0;
}

/*********** FEATURE 3 - REDIRECTING OUTPUT FROM STDIN/STDOUT TO FILE ************/
int redir(struct command_t *cmd){
	int redirect = 0;
	int i;
	for(i=0; i<cmd->argc; i++){
		if(!strcmp(cmd->argv[i], ">")){
			redirect = 1;
		}
	}
	return redirect;
}