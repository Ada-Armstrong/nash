#ifndef __MY_EXECUTE_H__
#define __MY_EXECUTE_H__

#include <assert.h>
#include <sys/wait.h>
#include <stdio.h>
#include "command.h"

extern const int EXIT_SHELL;

int execute(struct cmd_array *cmds);

#endif
