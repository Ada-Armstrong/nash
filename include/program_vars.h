#ifndef __NASH_PROGRAM_H__
#define __NASH_PROGRAM_H__

#include <stdio.h>

struct program_vars {
	// flag
	int interactive;
};

enum option {
	Interactive
};

static struct program_vars p_vars;


void init_program_vars();

int handle_args(char **arg); 

int get_option(enum option op);

#endif
