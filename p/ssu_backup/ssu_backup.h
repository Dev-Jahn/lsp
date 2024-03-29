#ifndef _SSU_BACKUP_H
#define _SSU_BACKUP_H	1

#include <stdlib.h>
#include <limits.h>

/*Option flags*/
#define OPT_D		0001	/*Directory backup*/
#define OPT_R		0002	/*Restore*/
#define OPT_M		0004	/*Backup only if modified*/
#define OPT_C		0010	/*Compare*/
#define OPT_N		0020	/*Number of files to maintain*/
#define OPT_S		0040	/*Strict modification check with SHA256*/
#define OPT_P		0100	/*Preserve backup data*/
#define OPT_H		0200	/*Reserve*/
#define OPT_W		0400	/*Reserve*/

/*Macro to get flag state*/
#define ON_D(flag)	(((flag) & OPT_D) == OPT_D)
#define ON_R(flag)	(((flag) & OPT_R) == OPT_R)
#define ON_M(flag)	(((flag) & OPT_M) == OPT_M)
#define ON_N(flag)	(((flag) & OPT_N) == OPT_N)
#define ON_C(flag)	(((flag) & OPT_C) == OPT_C)
#define ON_S(flag)	(((flag) & OPT_S) == OPT_S)
#define ON_P(flag)	(((flag) & OPT_P) == OPT_P)
#define ON_H(flag)	(((flag) & OPT_H) == OPT_H)
#define ON_W(flag)	(((flag) & OPT_W) == OPT_W)

extern unsigned int flag;
extern char targetpath[PATH_MAX];
extern char bakdirpath[PATH_MAX];
extern char fifopath[PATH_MAX];
extern int period;
extern size_t bakmax;

void usage(void);

#endif
