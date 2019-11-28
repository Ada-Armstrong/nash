#include "expansion.h"

static char *expand_recur(char *line, int len, int *added);

static char closing_char(char c)
{
	char ret;

	switch (c) {
	case '(':
		ret = ')';
		break;
	case '{':
		ret = '}';
		break;
	case '[':
		ret = ']';
		break;
	default:
		assert(0);
	}

	return ret;
}

static char *get_subshell_cmds(char *line, int *len)
{
	char open = *line;
	char close = closing_char(open);
	++line;
	int balance = 0;
	// find the length of the substring (until close)
	int i = 0;
	for (; balance > 0 || line[i] != close; ++i) {
		if (line[i] == '\0') {
			// couldn't find close
			return NULL;
		}
		if (line[1] == open) {
			++balance;
		} else if (line[i] == close) {
			--balance;
		}
	}
	int added = 0;
	char *cmds = expand_recur(line, i, &added);
	if (!cmds)
		return NULL;
	*len = i + added;

	return cmds;
}

static char *get_variable_string(char *line, int *len)
{
	int i = 0;
	while (line[i] && (line[i] != ' ' || line[i] != '\n'))
		++i;

	char *var = malloc(sizeof(*var) * (i + 1));
	if (!var)
		return NULL;

	for (int j = 0; j < i; ++j) {
		var[j] = line[j];
	}

	*len = i;
	var[i] = '\0';

	return var;
}

static int append_string(char **line, int i, int len,
		char *append, int a_len)
{
	char *tmp = realloc(*line, sizeof(**line) * (len + a_len));
	if (!tmp)
		return -1;
	*line = tmp;
	
	
	for (int j = i; j < i + a_len; ++j) {
		(*line)[j] = append[j - i];
	}

	return 1;
}

static int expand_subshell(char **line, int i, int len,
		char *subshell_line, int s_len)
{
	int out_len = 0;
	char *output = execute_subshell(subshell_line, &out_len);
	if (!output)
		return -1;

	if (append_string(line, i, len, output, out_len) < 0)
		return -1;
	free(output);

	return out_len;
}

// mutually recurses with get_subshell_cmds to expand subshells,
// base case is when line has no expandable expressions
static char *expand_recur(char *line, int len, int *added)
{
	char *expanded = malloc(sizeof(*expanded) * (len + 1));
	if (!expanded)
		return NULL;

	int backslash = 0;
	int index = 0; // used to iterate through line
	int exp_index = 0; // used to place next char into expanded

	while (index < len) {
		char c = *line;

		if (!backslash && c == '$' && line[1] == '(') {

			int c_len = 0;
			char *cmds = get_subshell_cmds(++line, &c_len);
			++index;
			if (!cmds) {
				free(expanded);
				return NULL;
			}
			int ret = expand_subshell(&expanded, exp_index,
					len + 1, cmds, c_len);
			free(cmds);
			if (ret < 0) {
				free(expanded);
				return NULL;
			}
			line += c_len + 2; // skip the expanded expression
			index += c_len + 2; // 2 is for brackets
			exp_index += ret;
			*added += ret;

		} else if (!backslash && c == '$' && line[1] != '\0') {

			int v_len = 0;
			char *var = get_variable_string(++line, &v_len);
			++index;
			if (!var) {
				free(expanded);
				return NULL;
			}
			char *replace = get_var(var);

			free(var);
			if (!replace)
				continue; // throw error instead possibly

			int r_len = strlen(replace);
			if (append_string(&expanded, exp_index, len + 1,
						replace, r_len) < 0) {
				free(expanded);
				return NULL;
			}
			line += v_len;
			index += v_len;
			exp_index += r_len;
			*added += r_len;

		} else if (!backslash && c == '$') {

			fprintf(stderr, "Unfinished expression '%c'\n", c);
			free(expanded);
			return NULL;

		} else {

			if (c == '\\') {
				backslash = 1;
				c = ' ';
			} else {
				backslash = 0;
			}

			expanded[exp_index] = c;
			++index;
			++exp_index;
			++line;
		}

	}

	expanded[exp_index] = '\0';

	return expanded;
}

char *expand(char *line, int len)
{
	int added = 0;
	return expand_recur(line, len, &added);
}
