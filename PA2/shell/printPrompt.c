/**
 * Once the prompt string is determined, the shell prints it to stdout whenever it is ready to
 * accept a command line.
 * For example, this function prints a prompt:
**/

#include "minishell.h"

void printPrompt(){
	/* Build the prompt string to have the current working directory */
	char promptString[MAX_PATH_LEN];
	char cwd[MAX_PATH_LEN];
	
	// get full pathname of current working directory
	if(!getcwd(cwd, sizeof(cwd))){
		perror("Unable to get current working dir\n");
	}else{
		// once obtained, get the basename for this full pathname
		strcpy(promptString, "<");
		strcat(promptString, basename(cwd));
		strcat(promptString, ">$");
		printf("%s", promptString);
	}
}