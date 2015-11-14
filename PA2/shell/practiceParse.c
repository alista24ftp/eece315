#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WHITESPACE " ,:\t\n"

int main(void){
	
	char *path = ".:/bin:/usr/bin alt_bin";
	char *path2 = "onlyonepath";
	char ptr[strlen(path)];
	char path2_cpy[strlen(path2)];
	char **dir, **dir2;
	
	int i = 0;
	
	dir = malloc(strlen(path) * sizeof(char *));
	dir2 = malloc(strlen(path2) * sizeof(char *));
			
	strcpy(ptr, path);
	// use strtok to parse "ptr"
	dir[0] = strtok(ptr, WHITESPACE);
	
	while(dir[i] != NULL){
		printf("%s\n", *(dir+i)); 
		i++;
		dir[i] = strtok(NULL, WHITESPACE);
	}
	
	// path2
	i = 0;
	strcpy(path2_cpy, path2);
	
	dir2[0] = strtok(path2_cpy, WHITESPACE);
	while(dir2[i] != NULL){
		printf("%s\n", *(dir2+i)); 
		i++;
		dir2[i] = strtok(NULL, WHITESPACE);
	}
	
	free(dir);
	free(dir2);
	
	return 0;
}