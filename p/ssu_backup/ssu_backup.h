#ifndef _MAIN_H
#define _MAIN_H	1

#include <stdlib.h>
#include <limits.h>

#define OPT_D		0001	/*Directory backup*/
#define OPT_R		0002	/*Restore*/
#define OPT_M		0004	/*Backup only if modified*/
#define OPT_C		0010	/*Compare*/
#define OPT_N		0020	/*Number of files to maintain*/
#define OPT_S		0040	/*Strict modification check with SHA-256*/
#define OPT_Y		0100	/*Reserve*/
#define OPT_Z		0200	/*Reserve*/
#define OPT_W		0400	/*Reserve*/
#define ON_D(flag)	(((flag) & OPT_D) == OPT_D)
#define ON_R(flag)	(((flag) & OPT_R) == OPT_R)
#define ON_M(flag)	(((flag) & OPT_M) == OPT_M)
#define ON_N(flag)	(((flag) & OPT_N) == OPT_N)
#define ON_C(flag)	(((flag) & OPT_C) == OPT_C)
#define ON_S(flag)	(((flag) & OPT_S) == OPT_S)
#define ON_Y(flag)	(((flag) & OPT_Y) == OPT_Y)
#define ON_Z(flag)	(((flag) & OPT_Z) == OPT_Z)
#define ON_W(flag)	(((flag) & OPT_W) == OPT_W)
#define SMODE 0664

extern int flag;
extern char execname[NAME_MAX];
extern char targetpath[PATH_MAX];
extern char bakdirpath[PATH_MAX];
extern char logdirpath[PATH_MAX];
extern int period;
extern size_t bakmax;
int setopt(int argc, char *argv[]);

#endif
