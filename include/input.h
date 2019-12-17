#ifndef __MY_INPUT_H__
#define __MY_INPUT_H__ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "smart_string.h"

/* Reads input from stdin until a newline is reached. Continues reading if
 * the last line ended with one of the strings in cont_strings. prompt is
 * printed if another line has to be read. cont_strings should be NULL
 * terminated. Client is responisble for freeing memory.
 */
char *read_input(char *prompt, int interactive);

#endif
