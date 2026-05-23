#include "main.h"

/**
 * _setenv - updates an environment variable in environ
 * @name: variable name
 * @value: new value
 */
void _setenv(char *name, char *value)
{
	int i = 0;
	size_t len = strlen(name);
	char *new_entry;

	new_entry = malloc(len + strlen(value) + 2);
	if (!new_entry)
		return;
	sprintf(new_entry, "%s=%s", name, value);
	while (environ && environ[i])
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
		{
			environ[i] = new_entry;
			return;
		}
		i++;
	}
}

/**
 * builtin_cd - implements the cd builtin command
 * @argv: argument vector
 * Return: 0 on success, 1 on failure
 */
int builtin_cd(char **argv)
{
	char *target = NULL;
	char cwd[1024];
	char *home, *prev;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("cd: getcwd");
		return (1);
	}
	if (!argv[1] || argv[1][0] == '\0')
	{
		home = _getenv("HOME");
		if (!home)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
		target = home;
	}
	else if (strcmp(argv[1], "-") == 0)
	{
		prev = _getenv("OLDPWD");
		if (!prev)
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
			return (1);
		}
		target = prev;
		printf("%s\n", target);
	}
	else
	{
		target = argv[1];
	}
	if (chdir(target) == -1)
	{
		fprintf(stderr, "cd: can't cd to %s\n", target);
		return (1);
	}
	_setenv("OLDPWD", cwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		_setenv("PWD", cwd);
	return (0);
}
