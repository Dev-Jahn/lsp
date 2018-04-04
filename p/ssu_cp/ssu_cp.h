#ifndef _SSU_CP_H
#define _SSU_CP_H	1

#define OPT_S		0001
#define OPT_I		0002
#define OPT_L		0004
#define OPT_N		0010
#define OPT_P		0020
#define OPT_R		0040
#define OPT_D		0100
#define OPT_G		0200
#define ON_S(flag)	(((flag) & OPT_S) == OPT_S)
#define ON_I(flag)	(((flag) & OPT_I) == OPT_I)
#define ON_L(flag)	(((flag) & OPT_L) == OPT_L)
#define ON_N(flag)	(((flag) & OPT_N) == OPT_N)
#define ON_P(flag)	(((flag) & OPT_P) == OPT_P)
#define ON_R(flag)	(((flag) & OPT_R) == OPT_R)
#define ON_D(flag)	(((flag) & OPT_D) == OPT_D)
#define ON_G(flag)	(((flag) & OPT_G) == OPT_G)

extern int flag;

#endif
