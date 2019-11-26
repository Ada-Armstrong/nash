#include <stdio.h>
#include "input.h"
#include "expansion.h"
#include "tokenize.h"
#include "execute.h"

int handle_args(char **arg); 
int run_subshell_mode(char *line);

int main(int argc, char *argv[])
{
	if (argc > 1 && !handle_args(++argv))
		return 1;

	char *cont_strings[] = {"\\", "&&", "||", "|", NULL};
	
	char cwd[1024];
	char *line;

	while (1) {
		getcwd(cwd, sizeof(cwd));
		// should check if cwd is NULL
		printf("%s$ ", cwd);

		int len = 0;
		line = read_input("> ", cont_strings, &len);
		if (!line)
			return 1;

		char *expanded = expand(line, len);
		if (!expanded) {
			free(line);
			return 1;
		}
		free(line);

		int num_tokens= 0;
		char **tokens = tokenize(expanded, " \t\n", &num_tokens);
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

int handle_args(char **args)
{
	char *first = *args;
	if (first[0] != '-') {
		fprintf(stderr, "'%s': Invalid argument\n", first);
		return 0;
	}
	++first;

	switch (*first) {
	case 'c':
		return !run_subshell_mode(args[1]);
	default:
		fprintf(stderr, "'%s': Invalid argument\n", *args);
	}

	return 0;
}

int run_subshell_mode(char *line)
{
	int len = 0;
	char **tokens = tokenize(line, " ", &len);
	if (!tokens)
		return 1;

	struct cmd_array *cmds = create_cmd_array(tokens);
	if (!cmds)
		return 1;

	execute(cmds);

	destroy_tokens(tokens);
	destroy_cmd_array(cmds);

	exit(0);
}
