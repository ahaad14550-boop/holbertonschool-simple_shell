#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/**
 * main - entry point for simple shell
 * @ac: argument count
 * @av: argument vector
 * @env: environment variables
 *
 * Return: 0 on success
 */
int main(int ac, char **av, char **env)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	int status, i;
	pid_t child_pid;
	char *token;
	char *argv[1024];

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
		i = 0;
		token = strtok(line, " \n\t\r");
		while (token != NULL)
		{
			argv[i++] = token;
			token = strtok(NULL, " \n\t\r");
		}
		argv[i] = NULL;
		if (argv[0] == NULL)
			continue;
		child_pid = fork();
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
			wait(&status);
	}
	free(line);
	return (0);
}
