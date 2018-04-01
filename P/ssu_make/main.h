#ifndef _MAIN_H
#define _MAIN_H		1

#include "struct.h"
#define OPT_M		0001
#define OPT_H		0002
#define OPT_S		0004
#define OPT_T		0010
#define OPT_F		0020
#define OPT_C		0040
#define OPT_P		0100
#define OPT_D		0200
#define ON_M(flag)	(((flag) & OPT_M) == OPT_M)
#define ON_H(flag)	(((flag) & OPT_H) == OPT_H)
#define ON_S(flag)	(((flag) & OPT_S) == OPT_S)
#define ON_T(flag)	(((flag) & OPT_T) == OPT_T)
#define ON_F(flag)	(((flag) & OPT_F) == OPT_F)
#define ON_C(flag)	(((flag) & OPT_C) == OPT_C)
#define ON_P(flag)	(((flag) & OPT_P) == OPT_P)
#define ON_D(flag)	(((flag) & OPT_D) == OPT_D)

extern int flag;
extern List execed;
int setopt(int argc, char * const argv[]);
void checkopt();

#endif
