#ifndef _PARSER_H_
#define _PARSER_H_

#include "struct.h"

COMMAND *parse_cmd(int argc, char *argv[]);
int compare(const char *patten, const char *string);
char *trim(const char *pattern, const char *string);
#endif
