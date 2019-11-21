#ifndef __MY_CMD_H__
#define __MY_CMD_H__

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum s_token {
	PIPE = 0,
	AND,
	OR,
	SEMI,
	NONE
};

struct cmd {
	char **tokens;
	enum s_token split_token;
	int negate;
	int read_fid;
};

struct cmd_array {
	struct cmd **array;
	unsigned int len;
	unsigned int index;
};


struct cmd_array *create_cmd_array(char **tokens);

void destroy_cmd_array(struct cmd_array *a);

#endif
