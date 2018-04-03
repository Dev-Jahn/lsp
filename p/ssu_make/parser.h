#ifndef _PARSER_H
#define _PARSER_H 1

#include <stdlib.h>
#include "struct.h"

extern List entered;
Command *parse_Cmd(int argc, char *argv[]);
Block *parse_Tree(int filedes, const char *target, TNode *parent);

#endif
