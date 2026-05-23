#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

extern char **environ;

char *_get_path(void);
char *find_path(char *cmd);
char *_getenv(const char *name);
void _setenv(char *name, char *value);
int builtin_cd(char **argv);
int execute_cmd(char **argv, char **av, int count, char **env);
int handle_operators(char *line, char **av, int count, char **env);

#endif
