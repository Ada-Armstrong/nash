#ifndef __NASH_ALIAS_H__
#define __NASH_ALIAS_H__

#define _GNU_SOURCE
#include <search.h>
#include <stdio.h>
#include <stdlib.h>

int init_alias(void);

int add_alias(char *var, char *cmd);

char *get_alias(char *var);

void destroy_alias(void);

#endif
