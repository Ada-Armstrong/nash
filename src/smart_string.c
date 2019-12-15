#include "smart_string.h"

struct s_string {
	char *data;
	unsigned int index;
	unsigned int max_len;
};

string create_s_string(char *input)
{
	string s = malloc(sizeof(*s));
	if (!s)
		return NULL;
	s->index = !input ? 0 : strlen(input);
	s->max_len = !input ? 8 : strlen(input) + 1;

	s->data = malloc(sizeof(*(s->data)) * s->max_len);
	if (!s->data) {
		free(s);
		return NULL;
	}

	if (input)
		strcpy(s->data, input);

	return s;
}

void destroy_s_string(string s)
{
	if (s) {
		if (s->data)
			free(s->data);
		free(s);
	}
}

int append_s_string(string s, char c)
{
	if (s->index >= s->max_len - 1) {
		s->max_len *= 2;
		char *tmp = realloc(s->data, sizeof(*(tmp)) * s->max_len);
		if (!tmp)
			return 0;
		s->data = tmp;
	}

	s->data[s->index++] = c;
	s->data[s->index] = '\0';
	return 1;
}

unsigned int size_s_string(string s)
{
	if (!s)
		return 0;
	return s->index;
}

char *convert_s_string(string s)
{
	if (!s)
		return NULL;
	char *cs = s->data;
	s->data = NULL;
	return cs;
}

char *show_s_string(string s)
{
	if (s)
		return s->data;
}
