#include <stdio.h>
#include "program_vars.h"
#include "input.h"
#include "expansion.h"
#include "tokenize.h"
#include "execute.h"
#include "alias.h"

void print_prompt(char *ps1);

int main(int argc, char *argv[])
{
	init_program_vars();
	init_alias();

	if (argc > 1 && !handle_args(++argv))
		return 1;

	int status;
	char *line;

	do {
		if (get_option(Interactive))
			print_prompt("$ ");

		line = read_input("> ", get_option(Interactive));
		if (!line)
			return 1;

		int len = 0;
		char **tokens = tokenize(line, &len);
		free(line);
		if (!tokens)
			return 1;

		struct cmd_array *cmds = create_cmd_array(tokens);
		destroy_tokens(tokens);
		if (!cmds)
			return 1;

		status = execute(cmds);

		destroy_cmd_array(cmds);

	} while(status != EXIT_SHELL);

	destroy_alias();

	return 0;
}

void print_prompt(char *ps1)
{
	printf("%s", ps1);
}
