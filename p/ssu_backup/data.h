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
	char filename[NAME_MAX];	/*Basename of target*/
	char abspath[PATH_MAX];		/*Absolute path of target*/
	mode_t mode;				/*Mode of target*/
	time_t mtime_last;			/*Last mtime of target*/
#ifdef SHA
	char checksum_last[SHA256_DIGEST_LENGTH*2 + 1];
#endif
	Queue fileQue;				/*Queue to save backup file path*/
	char updateflag;			/*Flag to detect deletion*/
} BakEntry;

typedef struct _BakTable
{
	BakEntry *be;
	size_t cnt;
} BakTable;


void init_table(BakTable *table);
void load_table(BakTable *table, const char *abspath);
BakEntry *load_entry(BakTable *table, const char *abspath);
BakEntry *add_entry(BakTable *table, const char *abspath);
BakEntry *renew_entry(BakTable *table, const char *abspath);
int remove_entry(BakTable *table, const char *abspath);
BakEntry *search_entry(BakTable *table, const char *abspath);

void initQueue(Queue *q);
void enqueue(Queue *q, void *data);
void *dequeue(Queue *q);
void *peek(Queue *q);

int check_modified(const char *abspath, BakEntry *e);
void compare_bak(const char *abspath);
void restore_bak(const char *abspath);
void find_bak(const char *findpath, const char *hexname, Queue *q);

#endif
