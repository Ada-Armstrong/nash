#ifndef __NASH_EXECUTE_H__
#define __NASH_EXECUTE_H__

#include <assert.h>
#include <sys/wait.h>
#include <stdio.h>
#include "command.h"

extern const int EXIT_SHELL;

int execute(struct cmd_array *cmds);

char *execute_subshell(char *line);

#endif
