#include "subshell.h"

// temporary
char *nash_path = "/home/chris/Projects/Shell/nash/build-debug/nash";

static char *execute_subshell(char *line, int *len)
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
		char *nash[] = {nash_path, "-c", line, NULL};
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
	*len = i;
	new_line = tmp;
	new_line[i] = '\0';

	return new_line;
}

int expand_subshell(char **line, int i, int len, char *subshell_line, int s_len)
{
	int out_len = 0;
	char *output = execute_subshell(subshell_line, &out_len);
	if (!output)
		return -1;

	char *tmp = realloc(*line, sizeof(**line) * (len + out_len));
	if (!tmp)
		return -1;
	*line = tmp;
	
	
	for (int j = i; j < i + out_len; ++j) {
		(*line)[j] = output[j - i];
	}
	free(output);

	return out_len;
}
