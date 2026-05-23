#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

extern char **environ;

char *_getenv(const char *name);
char *build_path(char *dir, char *cmd);
char *get_path(char *command);




char *_getenv(const char *name);
void _setenv(char *name, char *value);
int builtin_cd(char **argv);
#endif
