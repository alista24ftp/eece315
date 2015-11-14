#include "minishell.h"

int main(void){
	
	// shell init
	int status, pid;
	char cmdLine[LINE_LEN], cl_copy[LINE_LEN];
	//char *pathv[MAX_PATH_LEN];
	char **pathv = (char **)malloc(MAX_PATHS * sizeof(char *));
	char *cmdPath;
	struct command_t *cmd = malloc(sizeof(struct command_t));
	int hasAmpersand = 0;
	int i;
		
	// get directory paths from PATH
	parsePath(pathv);
	
	while(1){
		printPrompt();
		readCommand(cmdLine);
		strcpy(cl_copy, cmdLine);
		hasAmpersand = parseCommand(cmdLine, cmd); // FEATURE 2
		
		/*********************** FEATURE 1 - Internal Commands ************************/
		
		// if user types "exit" or "quit", quit from the shell
		if(!strcmp(cmd->name, "exit") || !strcmp(cmd->name, "quit")){
			
			break;
		}
		
		// cd (change directories)
		if(!strcmp(cmd->name, "cd")){
			if(chdir(cmd->argv[1])){
				perror("change directory failed...");
			}
			free(cmd->name);
			continue;
		}
		
		// pwd (print working directory)
		if(!strcmp(cmd->name, "pwd")){
			system(cmd->name);
			free(cmd->name);
			continue;
		}
		
		/*********** FEATURE 3 - REDIRECTING COMMAND OUTPUT TO FILES ************/
		if(redir(cmd)){
			system(cl_copy);
			free(cmd->name);
			continue;
		}
		
		// get full pathname for the file
		if((cmdPath = lookupPath(cmd->argv, pathv)) == NULL){
			free(cmd->name);
			continue;
		}
		
		// create child and execute command
		pid = fork();
		if(pid < 0){
			perror("Error: failed to fork");
			exit(-1);
		}
		if(pid == 0){
			// child will execute command
			execv(cmdPath, cmd->argv);
		}
		
		/******************** FEATURE 2 - RUN AS BACKGROUND PROCESS IF COMMAND ENDS WITH '&' ************/
		// this is the parent process, and will wait for child to terminate
		if(!hasAmpersand){
			waitpid(pid, &status, WUNTRACED);
		}
		
		free(cmd->name);
		free(cmdPath);
	}
	
	// shell termination
	free(cmd->name);
	free(cmd);
	
	for(i=0; i<MAX_PATHS; i++){
		free(pathv[i]);
	}
	free(pathv);
	return 0;
}