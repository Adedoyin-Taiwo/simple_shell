#include "main.h"
#include <unistd.h>
/**
* main - prints a prompt for user
* @ac: argument count
* @av: argument vector
* Return: 0
*/

int main()
{
	char *last_slash, command[MAX_COMMAND_LENGTH];
	size_t len;
	pid_t pid;
	int val;
	struct stat file_stat;

	printf("$ ");
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
	if (stat(command, &file_stat) == -1)
	{
		perror("Error checking executable");
		exit(EXIT_FAILURE);
	}
	if (!(file_stat.st_mode & S_IXUSR))
	{
		fprintf(stderr, "Error: The file is not executable\n");
		exit(EXIT_FAILURE);
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
		last_slash = strrchr(command, '/');
		if (last_slash != NULL)
		{
			*last_slash = '\0';
			chdir(command);
		}

		val = execve(command, args, environ);
		if (val == -1)
		{
			perror("Error executing command");
			exit(EXIT_FAILURE);
		}
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
	return 0;
}
