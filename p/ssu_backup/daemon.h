#ifndef _DAEMON_H
#define _DAEMON_H

#include "data.h"

struct argstr
{
	char *abspath;
	char *bakdir;
};

extern BakTable table;
int daemon_init(void);
void *func_ptr(void *arg);

#endif
