#include "tokenize.h"

static int is_whitespace(char c)
{
	return c == ' ' || c == '\t';
}

static char *my_strtok(char *line)
{
	static char *input = NULL;
	if (line)
		input = line;
	if (!input || *input == '\0')
		return NULL;
	//skip whitespace
	while(*input && is_whitespace(*input))
		++input;

	string token = create_s_string(NULL);
	int flag_escape = 0;
	char c;
	
	while(*input && !is_whitespace(*input)) {
		c = *input;

		switch (c) {
		case '\\':
			if (flag_escape)
				goto regular;
			flag_escape = 1;
			break;
		case '\'':
		case '"':
			if (flag_escape)
				goto regular;
			++input;
			// skip until end of quote
			while(*input != c) {
				if (*input == '\0') {
					//error
					return NULL;
				}
				if (!append_s_string(token, *input)) {
					destroy_s_string(token);
					return NULL;
				}
				++input;
			}
			break;
		regular:
		default:
			flag_escape = 0;
			if (!append_s_string(token, c)) {
				destroy_s_string(token);
				return NULL;
			}
		}

		++input;
	}
	
	char *ctoken = convert_s_string(token);
	destroy_s_string(token);
	return ctoken;
}

char **tokenize(char *input, int *len)
{
       	int max_tokens = 10;
	char **tmp = NULL;
	char **tokens = calloc(max_tokens, sizeof(*tokens));
	if (!tokens)
		return NULL;

	int index = 0;
	char *token = my_strtok(input);

	while(token != NULL) {
		if (index >= max_tokens - 1) {
			max_tokens *= 2;
			tmp = realloc(tokens, sizeof(*tokens) * max_tokens);
			if (!tokens)
				return NULL;
			tokens = tmp;
		}
		tokens[index] = token;
		token = my_strtok(NULL);
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
