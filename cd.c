#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char **environ;

/**
 * _getenv - gets an environment variable value
 * @name: variable name
 * Return: pointer to value or NULL
 */
char *_getenv(char *name)
{
    int i = 0;
    size_t len = strlen(name);

    while (environ && environ[i])
    {
        if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
            return (environ[i] + len + 1);
        i++;
    }
    return (NULL);
}

/**
 * _setenv - updates PWD or OLDPWD in environ
 * @name: variable name (e.g. "PWD")
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
 * builtin_cd - implements cd builtin
 * @argv: argument vector
 * Return: 0 on success, 1 on failure
 */
int builtin_cd(char **argv)
{
    char *target = NULL;
    char *oldpwd = NULL;
    char cwd[1024];
    char *home, *prev;

    /* احفظ المجلد الحالي قبل التغيير */
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("cd: getcwd");
        return (1);
    }
    oldpwd = cwd;

    if (!argv[1] || argv[1][0] == '\0')
    {
        /* cd بدون args -> اذهب لـ HOME */
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
        /* cd - -> اذهب للمجلد السابق */
        prev = _getenv("OLDPWD");
        if (!prev)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return (1);
        }
        target = prev;
        printf("%s\n", target); /* bash تطبعه */
    }
    else
    {
        target = argv[1];
    }

    /* نفذ التغيير */
    if (chdir(target) == -1)
    {
        fprintf(stderr, "cd: can't cd to %s\n", target);
        return (1);
    }

    /* حدّث OLDPWD و PWD */
    _setenv("OLDPWD", oldpwd);

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        _setenv("PWD", cwd);

    return (0);
}
