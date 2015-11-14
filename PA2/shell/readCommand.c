/**
 * Getting the command line: 
 * To get a command line, the shell performs a blocking read
 * operation so that the process that executes the shell will be blocked until the user types a
 * command line in response to the prompt. When the command has been provided by the
 * user (and terminated with a NEWLINE character), the command line string is returned to
 * the shell. 
**/

#include "minishell.h"

void readCommand(char *buffer)
{
	fgets(buffer, LINE_LEN, stdin);
}