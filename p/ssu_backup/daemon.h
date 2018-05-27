#ifndef _DAEMON_H
#define _DAEMON_H

#include "data.h"

struct argstr
{
	char abspath[PATH_MAX];
	char bakdir[PATH_MAX];
};

extern BakTable table;
int daemon_init(void);

#endif
