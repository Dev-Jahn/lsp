#ifndef _UTIL_H
#define _UTIL_H

#include <sys/types.h>

ssize_t strtohex(const char *str, char *buf, size_t bufsize);
ssize_t hextostr(const char *str, char *buf, size_t bufsize);
ssize_t makename(const char *pathname, char *buf, size_t bufsize);

#endif
