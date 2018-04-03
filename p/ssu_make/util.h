#ifndef _UTIL_H
#define _UTIL_H 1

#include "struct.h"

#define CHAR_PER_LINE 256
#define BUFFER_SIZE 1024

int compare(const char *pattern, const char *string);
char *trim(const char *pattern, const char *string);
Off_Pair regfind(int fd, const char *pattern);
char *sreplace(const char *src, const char *pat, const char *rep);
char *tospace(const char *str);

#endif
