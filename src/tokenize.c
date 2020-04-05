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

	char *final_token;
	string token = create_s_string(NULL);
	int flag_escape = 0;
	char c;
	
	while(*input && !is_whitespace(*input)) {
		c = *input;
		if (flag_escape)
			goto regular;

		// scary switch statement
		switch (c) {
		case '\\':
			if (size_s_string(token) == 0
					&& is_whitespace(*(input + 1))) {
				++input;
				while(*input && is_whitespace(*input))
					++input;
				continue;
			} else if (is_whitespace(*(input + 1))) {
				goto finished;
			}
			flag_escape = 1;
			if (!append_s_string(token, c))
				goto error;
			break;
		case ';':
			if (size_s_string(token) != 0)
				goto finished;
			if (!append_s_string(token, c))
				goto error;
			++input;
			goto finished;
		case '|':
		case '&':
			if (size_s_string(token) != 0)
				goto finished;
			if (!append_s_string(token, c))
				goto error;
			++input;
			if (*input == c) {
				if (!append_s_string(token, c))
					goto error;
				++input;
			}
			goto finished;
		case '$':
			if (!append_s_string(token, c))
				goto error;
			++input;
			c = ')';
			if (*input != '(' || !append_s_string(token, *input))
				goto error;
			++input;
			// skip until ) 
			while(*input != c) {
				if (*input == '\0')
					goto error;
				if (!append_s_string(token, *input))
					goto error;
				++input;
			}
			if (!append_s_string(token, c))
				goto error;
			++input;
			goto finished;
		case '\'':
		case '"':
			if (!append_s_string(token, c))
				goto error;
			++input;
			// skip until end of quote
			while(*input != c) {
				if (*input == '\0')
					goto error;
				if (!append_s_string(token, *input))
					goto error;
				++input;
			}
			if (!append_s_string(token, c))
				goto error;
			break;
		regular:
		default:
			flag_escape = 0;
			if (!append_s_string(token, c))
				goto error;
		}

		++input;
	}
finished:
	final_token = convert_s_string(token);
	destroy_s_string(token);
	return final_token;
error:
	fprintf(stderr, "nash: syntax error near unexpected token `%c`", c);
	destroy_s_string(token);
	return NULL;
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
			if (!tmp)
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
