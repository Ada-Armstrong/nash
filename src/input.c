#include "input.h"

static char *final_word(char *string, int end)
{
	// skip trailing whitespace
	while (end >= 0  && string[end] == ' ') {
		--end;
	}
	
	int start = end;
	for (; start >= 0; --start) {
		if (string[start] == ' ') {
			break;
		}
	}
	++start;

	char *final = malloc(sizeof(*final) * (end - start + 2));
	if (!final) {
		// memory allocation error
		return NULL;
	}

	int i = start;
	for (; i <= end; ++i) {
		final[i - start] = string[i];
	}

	final[i - start] = '\0';

	return final;
}

char *read_input(char *prompt, char **cont_strings)
{
	int i = 0;
	int maxlen = 10;
	char *line = malloc(sizeof(*line) * maxlen);
	char *tmp = NULL;
	char c;

	int finished = 0;	
	while (1) {
		while (1) {
			c = getc(stdin);
			if (c == EOF || c == '\n')
				break;

			if (i >= maxlen - 1) {
				maxlen *= 2;
				tmp = realloc(line, sizeof(*line) * maxlen);
				if (!tmp) {
					free(line);
					return NULL;
				}
				line = tmp;
			}

			line[i] = c;
			++i;
		}

		line[i] = '\0';
		char *final = final_word(line, i - 1);

		finished = 1;
		char **strings = cont_strings;

		for (; *strings; ++strings) {
			if (strcmp(final, *strings) == 0) {
				finished = 0;
				break;
			}
		}

		free(final);
		if (finished)
			break;
		printf("%s", prompt);
	}

	tmp = realloc(line, sizeof(*line) * i + 1);
	if (!tmp) {
		free(line);
		return NULL;
	}

	return tmp;
}

