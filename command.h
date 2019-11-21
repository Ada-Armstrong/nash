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

enum io_redirection {
	NONE = 0,
	IN,
	OUT,
	APPEND_OUT,
	IN_OUT
};

struct cmd {
	// first token is the command, rest are args
	char **tokens;
	enum s_token split_token;
	// 1 if ! is the first token before the command
	int negate;
	// used for a chain of pipes, the new input file descriptor
	int read_fid;
	enum io_redirection dir;
	char *redir_filename;
};

struct cmd_array {
	struct cmd **array;
	unsigned int len;
	unsigned int index;
};


struct cmd_array *create_cmd_array(char **tokens);

void destroy_cmd_array(struct cmd_array *a);

#endif
