#include "alias.h"

static const size_t SIZE = 512;

int init_alias(void)
{
	return hcreate(SIZE);
}

int add_alias(char *var, char *cmd)
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

static char *malloc_cpy(char *string)
{
	int len = strlen(string);
	char *new = malloc(sizeof(*new) * (len + 1));
	return strcpy(new, string);
}

char *get_alias(char *var)
{
	ENTRY e;
	e.key = var;
	ENTRY *ret = hsearch(e, FIND);
	return ret ? malloc_cpy(ret->data) : NULL;
}

void destroy_alias(void)
{
	// leaks memory if there are elements present in the table
	hdestroy();
}
