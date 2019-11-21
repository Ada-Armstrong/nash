#include <stdio.h>
#include "input.h"
#include "tokenize.h"
#include "execute.h"

int main()
{
	// if one of cont_strings is the last word in a line continue reading
	char *cont_strings[] = {"\\", "&&", "||", "|", NULL};
	
	const size_t size = 1024;
	char cwd[size];

	int running = 1;

	while (running) {
		getcwd(cwd, sizeof(cwd));
		// should check if cwd is NULL
		printf("%s$ ", cwd);

		char *string = read_input("> ", cont_strings);

		int len = 0;
		char **tokens = tokenize(string, " ", &len);

		struct cmd_array *cmds = create_cmd_array(tokens);

		if (execute(cmds) == EXIT_SHELL)
			running = 0;

		destroy_cmd_array(cmds);
		destroy_tokens(tokens);
		free(string);
	}

	return 0;
}
