#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdlib.h>
#include "struct.h"

Command *parse_Cmd(int argc, char *argv[]);
Block *parse_Tree(int filedes, const char *target, TNode *tn);

#endif
