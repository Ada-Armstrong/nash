#include "tokenize.h"

char **tokenize(char *input, const char *delim, int *len)
{
       	int max_tokens = 10;
	char **tmp = NULL;
	char **tokens = calloc(max_tokens, sizeof(*tokens));
	if (!tokens)
		return NULL;

	int index = 0;
	char *token = strtok(input, delim);

	while(token != NULL) {
		if (index >= max_tokens - 1) {
			max_tokens *= 2;
			tmp = realloc(tokens, sizeof(*tokens) * max_tokens);
			if (!tokens)
				return NULL;
			tokens = tmp;
		}
		char *cpy = malloc(sizeof(*cpy) * (strlen(token) + 1));
		if (!cpy) {
			destroy_tokens(tokens);
			return NULL;
		}
		tokens[index] = strcpy(cpy, token);
		token = strtok(NULL, delim);
		++index;
	}

	tmp = realloc(tokens, sizeof(*tokens) * (index + 1));
	if (!tmp) {
		destroy_tokens(tokens);
		return NULL;
	}
	tokens = tmp;

	tokens[index] = NULL;
	*len = index;

	return tokens;
}

void destroy_tokens(char **tokens)
{
	if (!tokens)
		return;
	for (int i = 0; tokens[i]; ++i) {
		free(tokens[i]);
	}
	free(tokens);
}
