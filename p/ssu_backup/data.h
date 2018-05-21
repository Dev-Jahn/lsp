#ifndef _DATA_H
#define _DATA_H

#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#ifdef SHA
#include <openssl/sha.h>
#endif

typedef struct _Node
{
	void *item;
	struct _Node *next;
} Node;

typedef struct _Queue
{
	Node *head;
	Node *tail;
	size_t size;
} Queue;

typedef struct _BakEntry
{
	char filename[NAME_MAX];
	char abspath[PATH_MAX];
	mode_t mode;
	size_t size;
	time_t mtime_last;
#ifdef SHA
	char checksum_last[SHA256_DIGEST_LENGTH*2 + 1];
#endif
	Queue fileQue;
} BakEntry;

typedef struct _BakTable
{
	BakEntry *be;
	size_t cnt;
} BakTable;


void init_table(BakTable *table);
BakEntry *add_bak(BakTable *table, const char *abspath);
BakEntry *renew_bak(BakTable *table, const char *abspath);
int remove_bak(BakTable *table, const char *abspath);
BakEntry *search_bak(BakTable *table, const char *abspath);

void initQueue(Queue *q);
void enqueue(Queue *q, void *data);
void *dequeue(Queue *q);
void *peek(Queue *q);

int check_modified(const char *abspath, BakEntry *e);
void compare_bak(const char *abspath);
void restore_bak(const char *abspath);
void find_bak(const char *findpath, const char *hexname, Queue *q);

#endif
