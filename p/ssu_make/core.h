#ifndef _CORE_H
#define _CORE_H		1

#include <sys/stat.h>
#include "struct.h"

void execute(TNode *tn);
int newest(TNode *tn, time_t mtime);

#endif
