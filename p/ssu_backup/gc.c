#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "gc.h"

Queue gcQue;
size_t used;
size_t limit;
float maintain_ratio;

void gc_start(size_t maxheap, float ratio)
{
	used = 0;
	limit = maxheap;
	maintain_ratio = ratio;
	initQueue(&gcQue);
}

int gc_check()
{
	if (used>=limit)
	{
		gc_flush();
		return 1;
	}
	else
		return 0;
}

void gc_flush(void)
{
	while (used > limit * maintain_ratio)
		gc_free();
}

void gc_enqueue(void *address, size_t size)
{
	struct memblock *mb = (struct memblock*)malloc(sizeof(struct memblock));
	mb->address = address;
	mb->size = size;
	enqueue(&gcQue, mb);
	used += size;
}

void gc_free(void)
{
	struct memblock *mb = dequeue(&gcQue);
	free(mb->address);
	used -= mb->size;
	free(mb);
}