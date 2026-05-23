#include "main.h"

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

int main(int ac, char **av, char **env)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	int i, count = 0, status = 0, wstatus;
	char *argv[1024];
	char *full_path;

	(void)ac;
	while (1)
	{
		count++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);
		nread = getline(&line, &len, stdin);
		if (nread == -1)
			break;
		if (strstr(line, "&&") || strstr(line, "||"))
		{
			status = handle_operators(line, av, count, env);
			continue;
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
			exit(status);
		}
		if (strcmp(argv[0], "env") == 0)
		{
			for (i = 0; env[i] != NULL; i++)
				printf("%s\n", env[i]);
			status = 0;
			continue;
		}
		if (strcmp(argv[0], "cd") == 0)
		{
			status = builtin_cd(argv);
			continue;
		}
		full_path = find_path(argv[0]);
		if (full_path)
		{
			if (fork() == 0)
			{
				if (execve(full_path, argv, env) == -1)
					exit(2);
			}
			else
			{
				wait(&wstatus);
				if (WIFEXITED(wstatus))
					status = WEXITSTATUS(wstatus);
			}
			free(full_path);
		}
		else
		{
			fprintf(stderr, "%s: %d: %s: not found\n", av[0], count, argv[0]);
			status = 127;
		}
	}
	free(line);
	return (status);
}
