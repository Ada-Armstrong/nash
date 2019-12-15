#include "program_vars.h"

void init_program_vars()
{
	p_vars.interactive = 1;
}

int handle_args(char **args)
{
	char *first = *args;
	if (first[0] != '-') {
		fprintf(stderr, "'%s': Invalid argument\n", first);
		return 0;
	}
	++first;

	switch (*first) {
	case 'c':
		p_vars.interactive = 0;
		return 1;
	default:
		fprintf(stderr, "'%s': Invalid argument\n", *args);
	}

	return 0;
}

int get_option(enum option op)
{
	switch (op) {
	case Interactive:
		return p_vars.interactive;
	}

	return 0;
}
