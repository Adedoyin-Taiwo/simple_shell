#include "main.h"
#include <unistd.h>

/**
* executeCommand - executes command
* @command: the command
* Return: nothing
*/

void executeCommand(const char *command)
{
	pid_t pid;
	int val;
	char *non_const_command = strdup(command);

	if (non_const_command == NULL)
		perror("Memory allocation error");
	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		return;
	}
	else if (pid == 0)
	{
		char *args[MAX_COMMAND_LENGTH];

		args[0] = non_const_command;
		args[1] = NULL;
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
			printf("Command executed successfully with
					exit status: %d\n", WEXITSTATUS(status));
		}
		else
		{
			printf("Command execution failed\n");
		}
	}
}

/**
* listExecutables - lists files in bin
* return: nothing
*/

void listExecutables(void)
{
	DIR *dir = opendir("/bin");
	struct dirent *entry;
	struct stat file_stat;

	if (!dir)
	{
		perror("Error opening directory");
		return;
	}
	while ((entry = readdir(dir)) != NULL)
	{
		size_t path_len = strlen("/bin/") + strlen(entry->d_name) + 1;
		char *file_path = (char *)malloc(path_len);

		if (file_path != NULL)
		{
			snprintf(file_path, path_len, "/bin/%s", entry->d_name);
			if (stat(file_path, &file_stat) == -1)
			{
				perror("Error checking file status");
				continue;
			}
			if (S_ISREG(file_stat.st_mode) && (file_stat.st_mode & S_IXUSR))
			{
				executeCommand(file_path);
			}
		}
	}
	closedir(dir);
}





/**
* main - prints a prompt for the user
* Return: 0
*/

int main(void)
{
	char *xit = "exit", *command = NULL;
	size_t n = 0;
	int len;

	do {
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
		command[len - 1] = '\0';
	if (strcmp(command, xit) == 0)
		break;
	if (strcmp(command, "/bin/lsbin") == 0)
	{
		listExecutables();
		continue;
	}
	executeCommand(command);
	} while (strcmp(command, xit) != 0);
	free(command);
	return (0);
}
