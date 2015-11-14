/**
 * This function searches the directories identified by the dir argument to see
 * if argv[0] (the file name) appears there.
 * Allocate a new string, place the full path name in it, then return the string. 
**/

#include "minishell.h"

char *lookupPath(char **argv, char **dir){
	
	char *result;
	char pName[MAX_PATH_LEN];
	int i;
	
	/* Check to see if file name is already an absolute path name */
	if(*argv[0] == '/') {
		// if file can be accessed, return the pathname
		if(!access(argv[0], F_OK)){
			result = (char *)malloc(MAX_PATH_LEN * sizeof(char));
			strcpy(result, argv[0]);
			return result;
		}
	}
	else{
		/* Look in PATH directories.
		Use access() to see if the file is in a dir.*/
		for(i = 0; i < MAX_PATHS; i++) {
			if(dir[i] != NULL){
				// check to see if "dir[i]/argv[0]" file can be accessed
				strcpy(pName, dir[i]);
				strcat(pName, "/");
				strcat(pName, argv[0]);
				if(!access(pName, F_OK)){
					result = (char *)malloc(MAX_PATH_LEN * sizeof(char));
					strcpy(result, pName);
					return result;
				}
			}
			else{
				break;
			}
		}
		
		// also try the command itself (eg. "./exe_file")
		strcpy(pName, argv[0]);
		if(!access(pName, F_OK)){
			result = (char *)malloc(MAX_PATH_LEN * sizeof(char));
			strcpy(result, pName);
			return result;
		}
	}
	
	/* if file name not found in any path variable till now then*/
	fprintf(stderr, "%s: command not found\n", argv[0]);
	return NULL;
}