#ifndef __SMART_STRING_H__
#define __SMART_STRING_H__

#include <stdlib.h>
#include <string.h>

typedef struct s_string *string;


string create_s_string(char *input);

void destroy_s_string(string s);

int append_s_string(string s, char c);

unsigned int size_s_string(string s);

char *convert_s_string(string s);

char *show_s_string(string s);

#endif
