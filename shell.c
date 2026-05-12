#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/**
 * main - Simple Shell 0.1 with robust space handling
 * @ac: arg count
 * @av: arg vector
 * @env: environment
 * Return: 0
 */
int main(int ac, char **av, char **env)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	int status;
	pid_t child_pid;
	char *token;
	char *argv[2];

	(void)ac;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		
		token = strtok(line, " \n\t\r");
		
		
		if (token == NULL)
			continue;

		argv[0] = token;
		argv[1] = NULL;

		child_pid = fork();
		if (child_pid == -1)
		{
			perror("fork");
			continue;
		}
		if (child_pid == 0)
		{
			if (execve(argv[0], argv, env) == -1)
			{
				
				perror(av[0]);
				free(line);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			wait(&status);
		}
	}
	free(line);
	return (0);
}
