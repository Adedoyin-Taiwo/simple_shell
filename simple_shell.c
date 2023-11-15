#include "main.h"

/**
* main - prints a prompt for user
* @ac: argument count
* @av: argument vector
* Return: 0
*/
int main()
{
	char command[MAX_COMMAND_LENGTH];
	size_t len;
	pid_t pid;
	int val;

	printf("apshell$ ");
	fflush(stdout);
	if (fgets(command, sizeof(command), stdin) == NULL)
	{
		printf("\nExiting shell...\n");
		return (1);
	}
        len = strlen(command);
        if (len > 0 && command[len - 1] == '\n')
	{
		command[len - 1] = '\0';
        }
	pid = fork();
        if (pid == -1)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		char *args[MAX_COMMAND_LENGTH];
                args[0] = command;
                args[1] = NULL;
		val = execvp(command, args);
		if (val == -1)
		{
			perror("Error executing command");
			exit(EXIT_FAILURE);
		}
		else
		{
			int status;
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
			{
				printf("Command executed successfully with exit status: %d\n", WEXITSTATUS(status));
			}
			else
			{
				printf("Command execution failed\n");
			}
		}
	}
	return 0;
}
