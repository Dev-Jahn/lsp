#ifndef _UTIL_H
#define _UTIL_H

#include <sys/types.h>
#include <dirent.h>
#ifdef SHA
#include <openssl/sha.h>
#endif

ssize_t strtohex(const char *str, char *buf, size_t bufsize);
ssize_t hextostr(const char *str, char *buf, size_t bufsize);
ssize_t timestamp(time_t when, char *buf, size_t  bufsize, const char *format);
time_t makename(const char *pathname, char *buf, size_t bufsize);
#ifdef SHA
int sha256_file(const char *pathname, char output[SHA256_DIGEST_LENGTH*2+1]);
#endif
int findpid(const char *procname, int *procs, size_t size);
int filter_default(const struct dirent *dir);

#endif
