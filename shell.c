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
 * Return: pointer to the PATH value, or NULL if not found.
 */
char *_get_path(void)
{
	int i = 0;

	while (environ && environ[i])
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
 * Return: full path of the command, or NULL if not found.
 */
char *find_path(char *cmd)
{
	char *path = _get_path();
	char *path_copy, *token, *full_path;
	struct stat st;

	if (!cmd)
		return (NULL);
	if (strchr(cmd, '/') && stat(cmd, &st) == 0)
		return (strdup(cmd));
	if (!path || path[0] == '\0')
		return (NULL);
	path_copy = strdup(path);
	token = strtok(path_copy, ":");
	while (token)
	{
		full_path = malloc(strlen(token) + strlen(cmd) + 2);
		if (full_path)
		{
			sprintf(full_path, "%s/%s", token, cmd);
			if (stat(full_path, &st) == 0)
			{
				free(path_copy);
				return (full_path);
			}
			free(full_path);
		}
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

/**
 * main - simple shell 0.4 with exit built-in
 * @ac: arg count
 * @av: arg vector
 * @env: environment
 * Return: 0
 */
int main(int ac, char **av, char **env)
{
	char *line = NULL, *full_path;
	size_t len = 0;
	ssize_t nread;
	int i, count = 0;
	char *argv[1024];

	(void)ac;
	while (1)
	{
		count++;
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
		if (strcmp(argv[0], "exit") == 0)
		{
			free(line);
			exit(0);
		}
		full_path = find_path(argv[0]);
		if (full_path)
		{
			if (fork() == 0)
			{
				if (execve(full_path, argv, env) == -1)
					exit(127);
			}
			else
				wait(NULL);
			free(full_path);
		}
		else
		{
			fprintf(stderr, "%s: %d: %s: not found\n", av[0], count, argv[0]);
			if (!isatty(STDIN_FILENO))
			{
				free(line);
				exit(127);
			}
		}
	}
	free(line);
	return (0);
}
