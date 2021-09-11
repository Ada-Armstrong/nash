#include "subshell.h"

#define NASH_PATH_LEN 256

char *execute_subshell(char *line, int *len)
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

		// get nash path
		char nash_path[NASH_PATH_LEN] = {0};
		readlink("/proc/self/exe", nash_path, NASH_PATH_LEN);

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
