#ifndef __TOKENIZE_H__
#define __TOKENIZE_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smart_string.h"

char **tokenize(char *input, int *len);

void destroy_tokens(char **tokens);

#endif
