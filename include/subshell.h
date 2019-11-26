#ifndef __NASH_SUBSHELL_H__
#define __NASH_SUBSHELL_H__

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int expand_subshell(char **line, int i, int len, char *subshell_line, int s_len);

#endif
