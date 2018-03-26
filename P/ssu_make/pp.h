#ifndef _PP_H_
#define _PP_H_

#include "struct.h"
#include "constants.h"

void preprocess(const char *pathname, Pair macro[MAX_MACRO]);
int incl(int fd);
#endif
