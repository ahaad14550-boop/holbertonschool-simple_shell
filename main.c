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

/* main.c */
char *_get_path(void);
char *find_path(char *cmd);

/* _getenv.c */
char *_getenv(const ch
