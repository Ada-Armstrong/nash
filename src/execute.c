#include "execute.h"

const int EXIT_SHELL = (int)'e' + (int)'x' + (int)'i' + (int)'t';

static int execute_builtin(struct cmd *c)
{
	int status;
	if (strcmp(c->tokens[0], "cd") == 0) {
		status = c->tokens[1] ? chdir(c->tokens[1])
			: chdir(getenv("HOME"));
	} else if (strcmp(c->tokens[0], "exit") == 0) {
		return EXIT_SHELL;
	} else {
		assert(0);
	}

	return c->negate ? !status : status;
}

static int execute_program(struct cmd *c)
{
	int status;
	pid_t child_pid = fork();
	
	if (child_pid == 0) {
		if (c->read_fid != STDIN_FILENO) {
			// set the reading end, for a chain of pipes
			dup2(c->read_fid, STDIN_FILENO);
			close(c->read_fid);
		}
		execvp(c->tokens[0], c->tokens);
		fprintf(stderr, "1: Unknown command %s\n", c->tokens[0]);
		exit(1);
	}

	waitpid(child_pid, &status, WUNTRACED);

	return c->negate ? !status : status;
}

static int is_builtin(struct cmd *c)
{
	return (strcmp(c->tokens[0], "cd") == 0)
		|| (strcmp(c->tokens[0], "exit") == 0);
}

static int execute_and(struct cmd_array *cmds)
{
	int retval = 0;
	struct cmd *c = cmds->array[cmds->index];
	if (is_builtin(c))
		retval = execute_builtin(c);
	else
		retval = execute_program(c);
	cmds->index += 1;

	if (retval == 0)
		return execute(cmds);
	return retval;
}

static int execute_or(struct cmd_array *cmds)
{
	int retval = 0;
	if (is_builtin(cmds->array[cmds->index]))
		retval = execute_builtin(cmds->array[cmds->index]);
	else
		retval = execute_program(cmds->array[cmds->index]);
	cmds->index += 1;

	if (retval)
		return execute(cmds);
	return retval;
}

static int execute_semi(struct cmd_array *cmds)
{
	if (is_builtin(cmds->array[cmds->index]))
		execute_builtin(cmds->array[cmds->index]);
	else
		execute_program(cmds->array[cmds->index]);
	cmds->index += 1;
	
	return execute(cmds);
}

static int execute_none(struct cmd_array *cmds)
{
	int retval = 0;
	if (is_builtin(cmds->array[cmds->index]))
		retval = execute_builtin(cmds->array[cmds->index]);
	else
		retval = execute_program(cmds->array[cmds->index]);
	cmds->index += 1;
	
	return retval;
}

static int execute_pipe(struct cmd_array *cmds)
{
	int status;

	if (is_builtin(cmds->array[cmds->index])) {
		return execute_semi(cmds);
	}

	int pipefid[2];
	pid_t first_pid;

	if (pipe(pipefid) < 0) {
		fprintf(stderr, "Pipe failed\n");
		exit(1);
	}

	struct cmd *c1 = cmds->array[cmds->index];
	struct cmd *c2 = cmds->array[cmds->index + 1];
	// set the next cmds stdin to pipe[0]
	c2->read_fid = pipefid[0];
	// a chain of pipes returns the exit status of the last command,
	// negating the first command in the chain negates the last status
	c2->negate = c1->negate;
	first_pid = fork();
	
	if (first_pid == 0) {
		close(pipefid[0]);
		if (c1->read_fid != STDIN_FILENO) {
			// set the reading end, for a chain of pipes
			dup2(c1->read_fid, STDIN_FILENO);
			close(c1->read_fid);
		}
		// set the writing end
		dup2(pipefid[1], STDOUT_FILENO);
		close(pipefid[1]);

		execvp(c1->tokens[0], c1->tokens);
		fprintf(stderr, "2: Unknown command %s\n", c1->tokens[0]);
		exit(1);
	}
	// next process won't need to write to this pipe
	close(pipefid[1]);

	cmds->index += 1;
	int retval = execute(cmds);

	waitpid(first_pid, &status, WUNTRACED);
	close(pipefid[0]);

	return retval;
}

int execute(struct cmd_array *cmds)
{
	if (!cmds)
		return -1;
	struct cmd *curr = cmds->array[cmds->index];

	int status;

	switch (curr->split_token) {
		case PIPE:
			status = execute_pipe(cmds);
			break;
		case AND:
			status = execute_and(cmds);
			break;
		case OR:
			status = execute_or(cmds);
			break;
		case SEMI:
			status = execute_semi(cmds);
			break;
		case NONE:
			status = execute_none(cmds);
			break;
		default:
			// shouldn't be possible
			assert(0);
	}

	return status;
}

char *execute_subshell(char *line)
{
	int status;
	int pipefid[2];

	if (pipe(pipefid) < 0) {
		fprintf(stderr, "Pipe failed\n");
		exit(1);
	}

	pid_t child_pid = fork();
	
	if (child_pid == 0) {
		dup2(pipefid[0], STDIN_FILENO);
		close(pipefid[0]);
		dup2(pipefid[1], STDOUT_FILENO);
		close(pipefid[1]);
		// run nash in subshell mode
		char *nash[] = {"nash", "-c", line, NULL};
		execvp(nash[0], nash);
		fprintf(stderr, "Subshell error\n");
		exit(1);
	}

	close(pipefid[1]);
	waitpid(child_pid, &status, WUNTRACED);

	char c[1];
	int maxlen = 2;
	int i = 0;
	char *new_line = malloc(sizeof(*new_line) * maxlen);
	char *tmp;
	if (!new_line) {
		close(pipefid[0]);
		close(pipefid[1]);
		return NULL;
	}
	// read has a few errors to consider
	while (read(pipefid[0], c, 1) > 0) {
		if (i >= maxlen - 1) {
			maxlen *= 2;
			tmp = realloc(new_line, sizeof(*new_line) * maxlen);
			if (!tmp) {
				fprintf(stderr,
					"execute_subshell: Memory error\n");
				free(new_line);
				return NULL;
			}
			new_line = tmp;
		}

		new_line[i] = c[0];
		++i;
	}

	close(pipefid[0]);

	tmp = realloc(new_line, sizeof(*new_line) * (i + 1));
	if (!tmp) {
		fprintf(stderr, "execute_subshell: Memory error\n");
		free(new_line);
		return NULL;
	}
	new_line = tmp;
	new_line[i] = '\0';

	return new_line;
}
