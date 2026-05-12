#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

/**
 * main - UNIX command line interpreter (Simple Shell 0.1)
 * @ac: argument count
 * @av: argument vector
 * @env: environment variables
 * Return: 0 on success
 */
int main(int ac, char **av, char **env)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	pid_t child_pid;
	int status;
	char *argv[2];

	(void)ac;

	while (1)
	{
		/* 1. Display Prompt */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		/* 2. Read Input */
		read = getline(&line, &len, stdin);

		/* 3. Handle EOF (Ctrl+D) */
		if (read == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			exit(EXIT_SUCCESS);
		}

		/* Remove newline character */
		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		/* 4. Fork and Execute */
		argv[0] = line;
		argv[1] = NULL;

		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Error");
			continue;
		}

		if (child_pid == 0)
		{
			/* Execute command */
			if (execve(argv[0], argv, env) == -1)
			{
				perror(av[0]);
				free(line);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			/* Parent waits for child */
			wait(&status);
		}
	}

	free(line);
	return (0);
}
