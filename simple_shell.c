#include "main.h"
#include <unistd.h>
**
* main - prints a prompt for user
* @ac: argument count
* @av: argument vector
* Return: 0
*/

int main()
{
	char *last_slash;
	char *command = NULL;
	size_t n = 0;
	pid_t pid;
	int val;
	int len;
	char *xit = "exit";
	struct stat file_stat;

	do	
	{
	printf("($) ");
	fflush(stdout);
	len = getline(&command, &n, stdin);
	if (len == -1)
	{
		if (feof(stdin))
		{
			printf("End of file reached. Exiting...\n");
			continue;
		}
		else
		{
			perror("getline error");
			continue;
		}
	}
        if (len > 0 && command[len - 1] == '\n')
	{
		command[len - 1] = '\0';
        }
	if (strcmp(command, xit) == 0)
		continue;
	if (stat(command, &file_stat) == -1)
	{
		perror("Error checking executable");
		continue;
	}
	if (!(file_stat.st_mode & S_IXUSR))
	{
		fprintf(stderr, "Error: The file is not executable\n");
		continue;
	}
	pid = fork();
        if (pid == -1)
	{

		continue;
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
			continue;
		}
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			printf("Command executed successfully with exit status: %d\n", WEXITSTATUS(status));
			continue;
		}
		else
		{
			printf("Command execution failed\n");
			continue;
		}
	}
	}while (strcmp(command, xit) != 0);
	return 0;
}
