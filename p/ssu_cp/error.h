#ifndef _ERROR_H
#define _ERROR_H

enum ErrCode { REOPT, COLOPT, LIMIT, MISSING, SAME, ISDIR, OPEN, STAT, CHMOD, CHOWN, UTIME, SYM };
void error(enum ErrCode err, const char *arg);

#endif
