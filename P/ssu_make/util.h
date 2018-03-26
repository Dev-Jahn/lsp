#ifndef _UTIL_H_
#define _UTIL_H_

#include "struct.h"

#define CHAR_PER_LINE 256
#define BUFFER_SIZE 1024

int compare(const char *pattern, const char *string);
char *trim(const char *pattern, const char *string);
Off_Pair regfind(int fd, const char *pattern);
ssize_t fconcat(int fd_dst, const char *pathname);
ssize_t freplace(int fd, const char *dst, const char *src);
#endif
