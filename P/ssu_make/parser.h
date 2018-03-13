#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdlib.h>
#include <regex.h>
#include "struct.h"

COMMAND *parse_cmd(int argc, char *argv[]);
int compare(const char *patten, const char *string);

#endif
