#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include "data.h"
#include "error.h"

void init_table(BakTable *table)
{
	table->be = (BakEntry*)malloc(0);
	table->cnt = 0;
}

BakEntry *add_bak(BakTable *table, const char *abspath)
{
	struct stat statbuf;
	BakEntry e;
	/*Expand the memory of backup table	*/
	table->be = realloc(table->be, sizeof(BakEntry)*(table->cnt+1));
	if (stat(abspath, &statbuf)<0)
		error(STAT, abspath);
	strcpy(e.filename, basename((char*)abspath));
	strcpy(e.abspath, abspath);
	e.mode = statbuf.st_mode;
	e.size = statbuf.st_size;
	e.mtime_last = statbuf.st_mtime;
#ifdef SHA
	sha256_file(abspath, e.checksum_last);
#endif
	initQueue(&(e.fileQue));

	memcpy(&(table->be[table->cnt]), &e, sizeof(BakEntry));
	table->cnt++;
	return &(table->be[table->cnt-1]);
}
BakEntry *renew_bak(BakTable *table, const char *abspath)
{
	struct stat statbuf;
	BakEntry *e = search_bak(table, abspath);
	if (stat(abspath, &statbuf)<0)
		error(STAT, abspath);
	e->mode = statbuf.st_mode;
	e->size = statbuf.st_size;
	e->mtime_last = statbuf.st_mtime;
#ifdef SHA
	sha256_file(abspath, e.checksum_last);
#endif
	return e;
}
/* ---------------------------------*/
/**
 * @brief Find the matching entry in the table.
 *
 * @param table Backup table
 * @param abspath absolute path of backup file
 *
 * @return matching entry
 */
/* ---------------------------------*/
BakEntry *search_bak(BakTable *table, const char *abspath)
{
	for (int i=0;i<(int)table->cnt;i++)
		if(strcmp(table->be[i].abspath,abspath) == 0)
			return &(table->be[i]);
	return NULL;
}

int remove_bak(BakTable *table, const char *abspath)
{
	BakEntry *e = search_bak(table, abspath);
	for (int i=0;i<(int)table->cnt;i++)
		if (e == &(table->be[i]))
		{
			memcpy(table->be+i,table->be+i+1,(table->cnt-i-1)*sizeof(BakEntry));
			table->cnt--;
			table->be = realloc(table->be,table->cnt*sizeof(BakEntry));
			return 0;
		}
	return -1;
}
void initQueue(Queue *q)
{
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
}

/* ---------------------------------*/
/**
 * @brief Insert node at the tail. 
 *
 * @param q Queue
 * @param item
 */
/* ---------------------------------*/
void enqueue(Queue *q, void *item)
{
	Node *nd = (Node*)malloc(sizeof(Node));
	nd->item = item;
	nd->next = NULL;
	if ((q->head==NULL)&&(q->tail==NULL))
		q->head = q->tail = nd;
	else
	{
		q->tail->next = nd;
		q->tail = nd;
	}
	q->size++;
}

/* ---------------------------------*/
/**
 * @brief Get and remove node from head.
 *
 * @param q Queue
 *
 * @return Item of head node
 */
/* ---------------------------------*/
void *dequeue(Queue *q)
{
	if ((q->size)==0)
		return NULL;
	else
	{
		void *item = q->head->item;
		Node *old = q->head;
		q->head = old->next;
		if (q->head == NULL)
			q->tail = NULL;
		free(old);
		(q->size)--;
		return item;
	}
}

void *peek(Queue *q)
{
	if ((q->size) == 0)
		return NULL;
	else
		return q->head->item;
}
/* ---------------------------------*/
/**
 * @brief Check if the file has been modified.
 *
 * @param abspath Absolute path of target file.
 *
 * @return If modified, 1. Not modified, 0. No file -1.
 */
/* ---------------------------------*/
int check_modified(const char *abspath, BakEntry *e)
{
	struct stat buf;
	if (stat(abspath, &buf)<0)
		return -1;
#ifdef SHA
	char checksum[SHA256_DIGEST_LENGTH*2+1];	
	sha256_file(abspath, checksum);
	if (strcmp(e->checksum_last, checksum) == 0)
#else
	if (buf.st_mtime == e->mtime_last)
#endif
		return 0;
	else
		return 1;
}
void compare_bak(void);
void restore_bak(void);
