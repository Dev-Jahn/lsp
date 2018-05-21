#ifndef _LOGGER_H
#define _LOGGER_H

#include <limits.h>
#include "data.h"

extern char logpath[PATH_MAX];
enum State { START, INIT, BACKUP, MODIFIED, DELETED, DELOLD, DIFF, RESTORE, EXIT };

void log_init(void);
void errlog(const char *format, ...);
void baklog(enum State st, BakEntry *bak);

#endif
