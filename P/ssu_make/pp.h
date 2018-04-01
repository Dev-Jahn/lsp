#ifndef _PP_H
#define _PP_H 1

#include "struct.h"
#include "constants.h"

extern List macroList;
int preprocess(const char *pathname, List *from_cmd);
int incl(int fd);
void syntax(const char *pathname);
void inner(int fd);

#endif
