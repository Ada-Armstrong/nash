#include "command.h"

char *Split_Tokens[] = {"|", "&&", "||", ";", NULL};

static struct cmd *create_cmd(char **tokens, enum s_token split_token, int negate)
{
	struct cmd *c = malloc(sizeof(*c));
	if (!c) {
		free(tokens);
		return NULL;
	}

	c->tokens = tokens;
	c->split_token = split_token;
	c->negate = negate;
	c->read_fid = STDIN_FILENO;

	return c;
}

static void destroy_cmd(struct cmd *c)
{
	if (!c)
		return;
	free(c->tokens);
	free(c);
}

static enum s_token is_split_token(char *token)
{
	if (!token)
		return NONE;
	for (int i = 0; Split_Tokens[i]; ++i) {
		if (strcmp(token, Split_Tokens[i]) == 0)
			return i;
	}
	return NONE;
}

static char **seperate_tokens(char **tokens, int *index, enum s_token *split)
{
	*split = NONE;
	*index = 0;
	// count the number of tokens for the command
	while (tokens[*index]) {
		*split = is_split_token(tokens[*index]);
		if (*split != NONE && *index == 0) {
			// split token cannot be first token
			return NULL;
		} else if (*split != NONE) {
			break;
		}
		*index += 1;
	}

	char **cmd_tokens = malloc(sizeof(*cmd_tokens) * (*index + 1));
	if (!cmd_tokens)
		return NULL;

	for (int i = 0; i < *index; ++i) {
		cmd_tokens[i] = tokens[i];
	}
	cmd_tokens[*index] = NULL;

	return cmd_tokens;
}

struct cmd_array *create_cmd_array(char **tokens)
{
	if (!tokens)
		return NULL;
	struct cmd_array *a = malloc(sizeof(*a));
	if (!a)
		return NULL;
	
	unsigned int maxlen = 5;
	a->array = malloc(sizeof(*(a->array)) * maxlen);
	if (!a->array)
		goto CLEANUP;
	struct cmd **tmp = NULL;

	a->len = 0;
	a->index = 0;

	enum s_token split = NONE;
	int index = 0;

	while (tokens) {
		int negate = strcmp(tokens[0], "!") == 0 ? 1 : 0;
		if (negate)
			++tokens;
		if (negate && split == PIPE) {
			// cannot negate in the middle of a pipe
			goto CLEANUP;
		}

		char **cmd_tokens = seperate_tokens(tokens, &index, &split);
		if (!cmd_tokens)
			goto CLEANUP;

		if (a->len >= maxlen) {
			maxlen *= 2;
			tmp = realloc(a->array, sizeof(*(a->array)) * maxlen);
			if (!tmp)
				goto CLEANUP;
			a->array = tmp;
		}

		a->array[a->len] = create_cmd(cmd_tokens, split, negate);
		a->len += 1;

		tokens = split != NONE ? tokens + index + 1 : NULL;
	}

	tmp = realloc(a->array, sizeof(*(a->array)) * a->len);
	if (!tmp)
		goto CLEANUP;
	a->array = tmp;

	return a;

CLEANUP:
	destroy_cmd_array(a);
	return NULL;
}

void destroy_cmd_array(struct cmd_array *a)
{
	if (!a)
		return;
	if (a->array) {
		for (int i = 0; i < a->len; ++i) {
			destroy_cmd(a->array[i]);
		}
		free(a->array);
	}
	free(a);
}
