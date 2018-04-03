#ifndef _IO_H
#define _IO_H 1

#include <stdlib.h>

ssize_t readLine(int filedes, void *buf_);
ssize_t delLine(int filedes);
ssize_t fconcat(int fd_dst, const char *pathname);
ssize_t freplace(int fd, const char *pattern, const char *src);
void erase(int filedes, off_t so, off_t eo);

#endif
