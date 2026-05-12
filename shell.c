#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

extern char **environ;

/**
 * _get_path - gets the PATH environment variable manually.
 *
 * Return: pointer to the PATH value, or NULL if not found.
 */
char *_get_path(void)
{
	int i = 0;

	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);
		i++;
	}
	return (NULL);
}

/**
 * find_path - looks for a command in the PATH directories.
 * @cmd: command to find.
 *
 * Return: full path of the command, or NULL if not found.
 */
char *find_path(char *cmd)
{
	char *path = _get_path();
	char *path_copy, *token, *full_path;
	struct stat st;

	if (!cmd)
		return (NULL);
	if (stat(cmd, &st) == 0)
		return (strdup(cmd));
	if (!path || path[0] == '\0')
		return (NULL);

	path_copy = strdup(path);
	if (!path_copy)
		return (NULL);
	token = strtok(path_copy, ":");
	while (token)
	{
		full_path = malloc(strlen(token) + strlen(cmd) + 2);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}
		sprintf(full_path, "%s/%s", token, cmd);
		if (stat(full_path, &st) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

/**
 * main - simple shell 0.3 compliant with Betty style.
 * @ac: argument count.
 * @av: argument vector.
 * @env: environment variables.
 *
 * Return: Always 0 on success.
 */
int main(int ac, char **av, char **env)
{
	char *line = NULL, *full_path;
	size_t len = 0;
	ssize_t nread;
	int status, i;
	pid_t child_pid;
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
		argv[i] = strtok(line, " \n\t\r");
		while (argv[i])
			argv[++i] = strtok(NULL, " \n\t\r");
		if (!argv[0])
			continue;
		full_path = find_path(argv[0]);
		if (full_path)
		{
			child_pid = fork();
			if (child_pid == 0)
			{
				if (execve(full_path, argv, env) == -1)
					perror(av[0]);
			}
			else
				wait(&status);
			free(full_path);
		}
		else
			perror(av[0]);
	}
	free(line);
	return (0);
}
