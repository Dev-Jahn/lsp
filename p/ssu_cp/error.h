#ifndef _ERROR_H
#define _ERROR_H

enum ErrCode { REOPT, COLOPT, SONLY, LIMIT, USAGE, MISSING, SAME, ISDIR, OPEN, STAT, CHMOD, CHOWN, UTIME, SYM, SCAN, MKDIR, ARGD };
void error(enum ErrCode err, const char *arg);

#endif
