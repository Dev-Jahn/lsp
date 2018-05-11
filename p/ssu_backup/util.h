#ifndef _UTIL_H
#define _UTIL_H

#include <sys/types.h>
#include <openssl/sha.h>

ssize_t strtohex(const char *str, char *buf, size_t bufsize);
ssize_t hextostr(const char *str, char *buf, size_t bufsize);
ssize_t timestamp(char *buf, size_t  bufsize, const char *format);
ssize_t makename(const char *pathname, char *buf, size_t bufsize);
int sha256_file(const char *pathname, char output[SHA256_DIGEST_LENGTH*2+1]);

#endif
