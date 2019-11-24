#include <stdio.h>
#include "input.h"
#include "tokenize.h"
#include "execute.h"

int main()
{
	// if one of cont_strings is the last word in a line,
	// continue reading input
	char *cont_strings[] = {"\\", "&&", "||", "|", NULL};
	
	char cwd[1024];
	char *line;

	while (1) {
		getcwd(cwd, sizeof(cwd));
		// should check if cwd is NULL
		printf("%s$ ", cwd);

		line = read_input("> ", cont_strings);
		if (!line)
			return 1;

		int len = 0;
		char **tokens = tokenize(line, " ", &len);
		free(line);
		if (!tokens)
			return 1;

		struct cmd_array *cmds = create_cmd_array(tokens);
		if (!cmds)
			return 1;

		if (execute(cmds) == EXIT_SHELL)
			break;

		destroy_tokens(tokens);
		destroy_cmd_array(cmds);
	}

	return 0;
}
