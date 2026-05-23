#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

/**
 * _find_env - Finds the index of an environment variable.
 * @name: The name of the variable to search for.
 *
 * Return: The index of the variable if found, or -1 if not found.
 */
int _find_env(const char *name)
{
	int i = 0;
	size_t len = 0;

	if (!name)
		return (-1);

	while (name[len])
		len++;

	while (environ[i])
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/**
 * my_setenv - Initializes a new environment variable, or modifies an existing.
 * @args: Array of arguments (args[1] is VARIABLE, args[2] is VALUE).
 *
 * Return: 0 on success, -1 on failure.
 */
int my_setenv(char **args)
{
	int idx, i = 0, len1 = 0, len2 = 0;
	char *new_var;

	if (!args[1] || !args[2])
	{
		write(STDERR_FILENO, "setenv: Invalid arguments\n", 26);
		return (-1);
	}
	while (args[1][len1])
		len1++;
	while (args[2][len2])
		len2++;
	new_var = malloc(len1 + len2 + 2);
	if (!new_var)
		return (-1);
	for (i = 0; i < len1; i++)
		new_var[i] = args[1][i];
	new_var[i++] = '=';
	for (idx = 0; idx < len2; idx++)
		new_var[i++] = args[2][idx];
	new_var[i] = '\0';
	idx = _find_env(args[1]);
	if (idx != -1)
		environ[idx] = new_var;
	else
	{
		i = 0;
		while (environ[i])
			i++;
		environ[i] = new_var;
		environ[i + 1] = NULL;
	}
	return (0);
}

/**
 * my_unsetenv - Removes an environment variable.
 * @args: Array of arguments (args[1] is VARIABLE).
 *
 * Return: 0 on success, -1 on failure.
 */
int my_unsetenv(char **args)
{
	int idx, i;

	if (!args[1])
	{
		write(STDERR_FILENO, "unsetenv: Invalid arguments\n", 28);
		return (-1);
	}

	idx = _find_env(args[1]);
	if (idx == -1)
		return (0);

	for (i = idx; environ[i] != NULL; i++)
	{
		environ[i] = environ[i + 1];
	}
	return (0);
}
