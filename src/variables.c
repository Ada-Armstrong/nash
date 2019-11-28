#include "variables.h"

static const size_t SIZE = 512;

int init_variables(void)
{
	return hcreate(SIZE);
}

int add_var(char *var, char *cmd)
{
	ENTRY e = {var, cmd};
	// check if var is already defined
	ENTRY *ret = hsearch(e, FIND);
	if (ret) {
		free(ret->key);
		free(ret->data);
	}
	if (!hsearch(e, ENTER)) {
		fprintf(stderr, "Failed to add '%s'\n", var);
		return 0;
	}

	return 1;
}

char *get_var(char *var)
{
	ENTRY e;
	e.key = var;
	ENTRY *ret = hsearch(e, FIND);
	return ret ? ret->data : NULL;
}

void destroy_variables(void)
{
	// leaks memory if there are elements present in the table
	hdestroy();
}
