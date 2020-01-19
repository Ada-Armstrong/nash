#include "builtin.h"

const int EXIT_SHELL = (int)'e' + (int)'x' + (int)'i' + (int)'t';

char *Builtin_Names [] = {"exit", "cd", "alias", NULL};
int (*Builtin_Funcs[])(struct cmd *) = {builtin_exit, builtin_cd,
		builtin_alias, NULL};

int builtin_exit(struct cmd *c)
{
	return EXIT_SHELL;
}

int builtin_cd(struct cmd *c)
{
	int status = c->tokens[1] ? chdir(c->tokens[1]) : chdir(getenv("HOME"));
	// add error message here
	return c->negate ? !status : status;
}

int builtin_alias(struct cmd *c)
{
	int argc = 0;
	for (char **t = c->tokens; *t; ++t) {
		++argc;
	}

	if (argc != 3) {
		fprintf(stderr, "Incorrect number of arguments for set\n");
		return 0;
	}

	char *key = malloc(sizeof(*key) * (strlen(c->tokens[1]) + 1));
	char *val = malloc(sizeof(*val) * (strlen(c->tokens[2]) + 1));
	if (!key || !val) {
		fprintf(stderr, "builtin_set: Out of memory error\n");
		exit(1);
	}
	strcpy(key, c->tokens[1]);
	strcpy(val, c->tokens[2]);

	int status = !add_alias(key, val);
	return c->negate ? !status : status;
}
