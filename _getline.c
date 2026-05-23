#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> /* هذا هو السطر الناقص الذي يحل المشكلة */

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
