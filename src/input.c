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
		fprintf(stderr, "final_word: Out of memory error\n");
		return NULL;
	}

	int i = start;
	for (; i <= end; ++i) {
		final[i - start] = string[i];
	}

	final[i - start] = '\0';

	return final;
}

static int my_readline(char **line, int *index, int *max)
{
	int i = *index;
	int maxlen = *max;
	char c;
	char *tmp;

	while (1) {
		c = getc(stdin);
		if (c == EOF || c == '\n')
			break;

		if (i >= maxlen - 1) {
			maxlen *= 2;
			tmp = realloc(*line, sizeof(**line) * maxlen);
			if (!tmp) {
				fprintf(stderr, "readline: Out of memory error\n");
				free(*line);
				return 0;
			}
			*line = tmp;
		}

		(*line)[i] = c;
		++i;
	}
	*index = i;
	*max = maxlen;
	return c == EOF ? -1 : 1;
}

char *read_input(char *prompt, char **cont_strings, int *len)
{
	int i = 0;
	int maxlen = 2;
	char *line = malloc(sizeof(*line) * maxlen);
	if (!line) {
		fprintf(stderr, "read_input: Out of memory error\n");
		exit(1);
	}
	char *tmp = NULL;

	int finished = 1;
	do {
		if (!finished)
			printf("%s", prompt);
		int retval = my_readline(&line, &i, &maxlen);
		
		if (retval == 0)
			return NULL;

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

		if (!finished && retval < 0) {
			fprintf(stderr,
				"Read EOF but last token was '%s'\n", final);
			return NULL;
		}
		free(final);
	} while (!finished);

	tmp = realloc(line, sizeof(*line) * i + 1);
	if (!tmp) {
		free(line);
		exit(1);
	}
	*len = i;

	return tmp;
}
