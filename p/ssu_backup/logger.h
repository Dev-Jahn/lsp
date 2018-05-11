#ifndef _LOGGER_H
#define _LOGGER_H

#include <limits.h>

extern char logpath[PATH_MAX];
enum State { MODIFIED, DELETED, DELOLD, EXIT };

void errlog(const char *format, ...);
void baklog(const char *format, ...);

#endif
