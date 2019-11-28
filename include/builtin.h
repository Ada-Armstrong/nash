#ifndef __NASH_BUILTIN_H__
#define __NASH_BUILTIN_H__

#include <stdio.h>
#include "command.h"
#include "variables.h"

extern const int EXIT_SHELL;

extern char *Builtin_Names[];
extern int (*Builtin_Funcs[])(struct cmd *);

int builtin_exit(struct cmd *c);

int builtin_cd(struct cmd *c);

int builtin_set(struct cmd *c);

#endif
