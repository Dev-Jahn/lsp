#ifndef _ERROR_H
#define _ERROR_H

enum ErrCode { REOPT, COLOPT, LIMIT, USAGE, MISSING, SAME, ISDIR, OPEN, STAT, CHMOD, CHOWN, UTIME, SYM, SCAN, MKDIR };
void error(enum ErrCode err, const char *arg);

#endif
