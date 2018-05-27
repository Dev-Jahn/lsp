#ifndef _GC_H
#define _GC_H

#include <stdlib.h>

void gc_start(size_t maxheap, float ratio);
int gc_check();
void gc_clear(void);
void gc_enqueue(void *address, size_t size);

#endif
