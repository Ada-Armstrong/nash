#ifndef __NASH_EXECUTE_H__
#define __NASH_EXECUTE_H__

#include <stdio.h>
#include <sys/wait.h>
#include <assert.h>
#include "command.h"
#include "builtin.h"

int execute(struct cmd_array *cmds);

#endif
