#ifndef __NASH_VARIABLES_H__
#define  __NASH_VARIABLES_H__

#define _GNU_SOURCE
#include <search.h>
#include <stdio.h>
#include <stdlib.h>

int init_variables(void);

int add_var(char *var, char *cmd);

char *get_var(char *var);

void destroy_variables(void);

#endif
