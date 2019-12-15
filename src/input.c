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

static int my_readline(string line)
{
	char c;

	while (1) {
		c = getc(stdin);
		if (c == EOF || c == '\n')
			break;

		append_s_string(line, c);
	}

	return c == EOF ? -1 : 1;
}

char *read_input(char *prompt, char **cont_strings, int *len)
{
	string s = create_s_string(NULL);
	if (!s) {
		fprintf(stderr, "read_input: Out of memory error\n");
		return NULL;
	}

	int finished = 1;

	do {
		if (!finished)
			printf("%s", prompt);

		int retval = my_readline(s);
		
		if (retval <= 0)
			return NULL;

		char *final = final_word(show_s_string(s), size_s_string(s) - 1);

		finished = 1;
		char **strings = cont_strings;

		for (; *strings; ++strings) {
			if (strcmp(final, *strings) == 0) {
				finished = 0;
				break;
			}
		}

		if (!finished && retval < 0) {
			fprintf(stderr, "EOF but last token was '%s'\n", final);
			return NULL;
		}
		free(final);

	} while (!finished);

	char *line = convert_s_string(s);
	destroy_s_string(s);

	return line;
}
