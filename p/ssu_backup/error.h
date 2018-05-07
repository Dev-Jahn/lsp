#ifndef _ERROR_H
#define _ERROR_H

enum ErrCode { NAMEMAX, PATHMAX };
void error(enum ErrCode err, const char *arg);

#endif
