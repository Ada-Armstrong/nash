#ifndef __TOKENIZE_H__
#define __TOKENIZE_H__

#include <stdlib.h>
#include <string.h>

char **tokenize(char *input, const char *delim, int *len);

void destroy_tokens(char **tokens);

#endif
