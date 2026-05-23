#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

if (strstr(line, "&&") || strstr(line, "||"))
{
    status = handle_operators(line, av, count, env);
    continue;
}

extern char **environ;

/* main.c */
char *_get_path(void);
char *find_path(char *cmd);
int handle_operators(char *line, char **av, int count, char **env);
/* _getenv.c */
char *_getenv(const ch
