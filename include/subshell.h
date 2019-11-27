#ifndef __NASH_SUBSHELL_H__
#define __NASH_SUBSHELL_H__

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

char *execute_subshell(char *line, int *len);

#endif
