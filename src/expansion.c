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
	// len is calculated incorrectly, doesn't consider call to expand
	*len = i + added;

	return cmds;
}

// mutually recurses with get_subshell_cmds to expand subshells,
// base case is when line has no expandable expressions
static char *expand_recur(char *line, int len, int *added)
{
	int new_len = len + 1;
	char *expanded = malloc(sizeof(*expanded) * new_len);
	if (!expanded)
		return NULL;

	int backslash = 0;
	char c;
	int index = 0; // used to iterate through line
	int exp_index = 0; // used to place next char into expanded

	while (index < len) {
		c = *line;

		if (!backslash && c == '$' && line[1] == '(') {

			int c_len = 0;
			char *cmds = get_subshell_cmds(++line, &c_len);
			++index;
			if (!cmds) {
				free(expanded);
				return NULL;
			}
			int ret = expand_subshell(&expanded, exp_index,
					new_len, cmds, c_len);
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

			// alias
			assert(0);

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
