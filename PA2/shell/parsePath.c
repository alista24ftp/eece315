/**
 * This function reads the PATH variable for this environment, then builds an
 * array, dirs[], of the directories in PATH 
**/

#include "minishell.h"

int parsePath(char *dirs[]){
	
	char *pathEnvVar;
	char *thePath;
	char *temp;
	int i;
	
	for(i=0; i<MAX_PATHS; i++){
		dirs[i] = NULL; /* set to null */
	}
	
	pathEnvVar = (char *) getenv ("PATH");
	thePath = (char *) malloc(strlen(pathEnvVar) + 1);
	strcpy(thePath, pathEnvVar);
	
	/* Loop to parse thePath. Look for a ':' delimiter between each path name. */
	i = 0;
	temp = strtok(thePath, ":");
	while(strlen(temp) > MAX_PATH_LEN){
		temp = strtok(NULL, ":");
		if(temp == NULL){
			free(thePath);
			return 0;
		} 
	}
	dirs[i] = (char *)malloc((strlen(temp) + 1)* sizeof(char));
	strcpy(dirs[i], temp);
	
	while(dirs[i] != NULL){
		i++;
		temp = strtok(NULL, ":");
		if(temp == NULL) break;
		while(strlen(temp) > MAX_PATH_LEN){
			temp = strtok(NULL, ":");
			if(temp == NULL){
				free(thePath);
				return 1;
			}
		}
		dirs[i] = (char *)malloc((strlen(temp) + 1)* sizeof(char));
		strcpy(dirs[i], temp);
		
	}
	
	free(thePath);
	return 1;
	
}