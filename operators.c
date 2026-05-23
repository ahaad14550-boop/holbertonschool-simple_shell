#include "main.h"

int execute_cmd(char **argv, char **av, int count, char **env)
{
	char *full_path;
	int status = 0, wstatus;
	int i;

	if (!argv[0])
		return (0);
	if (strcmp(argv[0], "exit") == 0)
		exit(status);
	if (strcmp(argv[0], "env") == 0)
	{
		for (i = 0; env[i]; i++)
			printf("%s\n", env[i]);
		return (0);
	}
	if (strcmp(argv[0], "cd") == 0)
		return (builtin_cd(argv));
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
	return (status);
}

int handle_operators(char *line, char **av, int count, char **env)
{
	char *tokens[1024];
	char *argv[1024];
	int i = 0, j = 0, status = 0;
	int tok_count = 0;
	char *copy, *cur, *next_and, *next_or;
	int is_and[1023];

	copy = strdup(line);
	cur = copy;
	while (cur && *cur)
	{
		next_and = strstr(cur, "&&");
		next_or = strstr(cur, "||");
		if (!next_and && !next_or)
		{
			tokens[tok_count] = strdup(cur);
			tok_count++;
			break;
		}
		if (next_and && (!next_or || next_and <= next_or))
		{
			*next_and = '\0';
			tokens[tok_count] = strdup(cur);
			is_and[tok_count] = 1;
			tok_count++;
			cur = next_and + 2;
		}
		else
		{
			*next_or = '\0';
			tokens[tok_count] = strdup(cur);
			is_and[tok_count] = 0;
			tok_count++;
			cur = next_or + 2;
		}
	}
	for (i = 0; i < tok_count; i++)
	{
		j = 0;
		argv[j] = strtok(tokens[i], " \n\t\r");
		while (argv[j])
			argv[++j] = strtok(NULL, " \n\t\r");
		if (i == 0)
			status = execute_cmd(argv, av, count, env);
		else if (is_and[i - 1] == 1)
		{
			if (status == 0)
				status = execute_cmd(argv, av, count, env);
		}
		else
		{
			if (status != 0)
				status = execute_cmd(argv, av, count, env);
		}
		free(tokens[i]);
	}
	free(copy);
	return (status);
}
