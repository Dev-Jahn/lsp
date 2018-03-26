#ifndef _UTIL_H_
#define _UTIL_H_

#include "struct.h"
#define CHAR_PER_LINE 256

int compare(const char *pattern, const char *string);
char *trim(const char *pattern, const char *string);
Off_Pair regfind(int fd, const char *pattern);
#endif
