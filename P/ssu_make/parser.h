#ifndef _PARSER_H_
#define _PARSER_H_
#include <stdlib.h>
#include "struct.h"

COMMAND *parse_cmd(int argc, char *argv[]);
BLOCK *parse_block(const char *fname);
TREE *dependTree(BLOCK *blks, size_t amount);
#endif
