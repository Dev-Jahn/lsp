#ifndef _PARSER_H_
#define _PARSER_H_
#include <stdlib.h>
#include "struct.h"

Command *parse_cmd(int argc, char *argv[]);
Block *parse_Block(const char *fname);
Tree *parse_tree(Block *blks, size_t amount);

#endif
