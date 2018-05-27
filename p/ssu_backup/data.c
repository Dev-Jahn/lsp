#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include "ssu_backup.h"
#include "logger.h"
#include "data.h"
#include "io.h"
#include "util.h"

/*Internal usage*/
static BakEntry *add_entry(BakTable *, const char *);
static void find_bak(const char *, const char *, Queue *);

/* ---------------------------------*/
/**
 * @brief Initialize as empty table.
 *
 * @param table Pointer of the backup table
 */
/* ---------------------------------*/
void init_table(BakTable *table)
{
	table->be = (BakEntry*)malloc(0);
	table->cnt = 0;
}

/* ---------------------------------*/
/**
 * @brief Make a backup table with existing backups.
 *
 * @param table Pointer to table
 * @param abspath Path of target
 */
/* ---------------------------------*/
void load_table(BakTable *table, const char *abspath)
{
	if (ON_D(flag))
	{
		struct dirent **de;
		int dirnum;
		if ((dirnum = scandir(abspath, &de, filter_default, alphasort)) < 0)
			error(SCAN, abspath);
		for (int i=0;i<dirnum;i++)
		{
			if (de[i]->d_ino == 0)
				continue;

			char path[PATH_MAX];
			strcpy(path, abspath);
			strcat(path, "/");
			strcat(path, de[i]->d_name);

			if (de[i]->d_type == DT_DIR)
				load_table(table, path);
			else
				load_entry(table, path);
		}

		for (int i=0;i<dirnum;i++)
			free(de[i]);
		free(de);
	}
	else
		load_entry(table, targetpath);
}

/* ---------------------------------*/
/**
 * @brief Make a backup entry with existing backups.
 *
 * @param table Pointer to table
 * @param abspath Path of target
 */
/* ---------------------------------*/
BakEntry *load_entry(BakTable *table, const char *abspath)
{
	char hexname[NAME_MAX+1];
	Queue q;
	initQueue(&q);
	strtohex(abspath, hexname, sizeof(hexname));

	/*Find the matching file and put it in the queue*/
	find_bak(bakdirpath, hexname, &q);

	BakEntry *e;
	e = add_entry(table, abspath);
	/*If found*/
	if (q.size > 0)
		/*Transfer to fileque in entry structure*/
		for (int i=0;i<(int)q.size;)
			enqueue(&e->fileQue, dequeue(&q));
	return e;
}

/* ---------------------------------*/
/**
 * @brief Add entry of the target file to the table.
 *
 * @param table Pointer of the backup table
 * @param abspath Absolute path of target file
 *
 * @return Pointer of added entry
 */
/* ---------------------------------*/
BakEntry *add_entry(BakTable *table, const char *abspath)
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
	e.mtime_last = statbuf.st_mtime;
#ifdef SHA
	sha256_file(abspath, e.checksum_last);
#endif
	initQueue(&(e.fileQue));
	e.updateflag = 1;

	memcpy(&(table->be[table->cnt]), &e, sizeof(BakEntry));
	table->cnt++;
	return &(table->be[table->cnt-1]);
}
BakEntry *renew_entry(BakTable *table, const char *abspath)
{
	struct stat statbuf;
	BakEntry *e = search_entry(table, abspath);
	if (stat(abspath, &statbuf)<0)
		error(STAT, abspath);
	e->mode = statbuf.st_mode;
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
 * @param abspath Absolute path of target file
 *
 * @return Matching entry
 */
/* ---------------------------------*/
BakEntry *search_entry(BakTable *table, const char *abspath)
{
	for (int i=0;i<(int)table->cnt;i++)
		if(strcmp(table->be[i].abspath,abspath) == 0)
			return &(table->be[i]);
	return NULL;
}

/* ---------------------------------*/
/**
 * @brief Remove entry from backup table
 *
 * @param table Backup table
 * @param abspath Absolute path of target file
 *
 * @return Success 0, else -1
 */
/* ---------------------------------*/
int remove_entry(BakTable *table, const char *abspath)
{
	BakEntry *e = search_entry(table, abspath);
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

/* ---------------------------------*/
/**
 * @brief Initialize queue
 *
 * @param q Queue
 */
/* ---------------------------------*/
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
 * @param item Pointer of item to put in
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

/* ---------------------------------*/
/**
 * @brief Peek head of the queue.
 *
 * @param q Queue
 *
 * @return Item of the head node
 */
/* ---------------------------------*/
void *peek(Queue *q)
{
	if ((q->size) == 0) return NULL;
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

/* ---------------------------------*/
/**
 * @brief Compare file with latest backup
 *
 * @param abspath Absolute path of target file
 */
/* ---------------------------------*/
void compare_bak(const char *abspath)
{
	char hexname[NAME_MAX+1];
	char *filename = basename((char*)abspath);
	Queue q;
	initQueue(&q);
	strtohex(abspath, hexname, sizeof(hexname));

	/*Find the matching file and put it in the queue*/
	find_bak(bakdirpath, hexname, &q);

	/*If found*/
	if (q.size > 0)
	{
		printf("\n[Compare with backup '%s_%s']\n",
				filename, getbtime((char*)q.tail->item));
		baklog(DIFF, NULL, filename, basename(q.tail->item));
		int status = 0;
		pid_t pid;
		if ((pid = fork()) == 0)
			execlp("diff", "diff", abspath, q.tail->item, NULL);
		else
			wait(&status);
		if (!WEXITSTATUS(status))
			printf("File is identical with last backup.\n");

		/*Free*/
		for (int i=0;i<(int)q.size;i++)
			free(dequeue(&q));
	}
	/*Not found*/
	else
		printf("There's no backup file of '%s'.\n", filename); 
	baklog(EXIT, NULL);
	exit(0);
}
/* ---------------------------------*/
/**
 * @brief Restore file from backup
 *
 * @param abspath Absolute path of target file
 */
/* ---------------------------------*/
void restore_bak(const char *abspath)
{
	char hexname[NAME_MAX+1];
	char *filename = basename((char*)abspath);
	errlog("%s",abspath);
	Queue q;
	initQueue(&q);
	strtohex(abspath, hexname, sizeof(hexname));

	/*Find the matching file and put it in the queue*/
	find_bak(bakdirpath, hexname, &q);

	/*If found*/
	if (q.size > 0)
	{
		int select;
		struct stat buf;
		Node *ptr = q.head;

		printf("\n[%s backup list]\n", filename);
		printf("0. Exit\n");
		/*Print backup file list and prompt*/
		for (int i=1;i<=(int)q.size;i++)
		{
			if (stat((char*)ptr->item, &buf)<0)
				error(STAT, (char*)ptr->item);
			printf("%d. %s_%s [size:%ld]\n",
					i, filename, getbtime((char*)ptr->item), buf.st_size);
			ptr = ptr->next;
		}
		printf("Input >> ");

		/*Get input from user and restore selected*/
		while (1)
		{
			scanf("%d", &select); 
			if (select == 0)
			{
				printf("No file selected.\n");
				break;
			}
			else if (select>0 && select<=(int)q.size)
			{
				Node *ptr = q.head;
				for (int i=1;i<select;i++)
					ptr = ptr->next;
				printf("Restoring backup...\n");
				printf("\n[%s]\n", filename);
				copy(ptr->item, abspath);	/*Restore*/
				cat(abspath);				/*Print*/
				baklog(RESTORE, NULL, filename, basename(ptr->item));
				break;
			}
			else
			{
				printf("Wrong input.\nInput >> ");
				getchar();
			}
		}
		/*Free*/
		for (int i=0;i<(int)q.size;i++)
			free(dequeue(&q));
	}
	/*Not found*/
	else
		printf("There's no backup file of '%s'.\n", filename); 
	baklog(EXIT, NULL);
	exit(0);
}

/* ---------------------------------*/
/**
 * @brief Find backup file of filename in findpath, enqueue it.
 *
 * @param findpath directory to find backup file.
 * @param filename hexadecimal part of "hex_timestamp"
 * @param q Queue to save the found items.
 */
/* ---------------------------------*/
void find_bak(const char *findpath, const char *hexname, Queue *q)
{
	struct dirent **de;
	int dirnum;
	if ((dirnum = scandir(findpath, &de, filter_default, alphasort))<0)
		error(SCAN, findpath);
	for (int i=0;i<dirnum;i++)
	{
		if (de[i]->d_ino == 0)
			continue;

		char subpath[PATH_MAX];
		strcpy(subpath, findpath);
		strcat(subpath, "/");
		strcat(subpath, de[i]->d_name);
		if (de[i]->d_type == DT_REG)
		{
			char *hex;
			if ((hex = gethexname(de[i]->d_name)) == NULL)
				continue;
			char *dpath= (char*)malloc(PATH_MAX);
			strcpy(dpath, findpath);
			strcat(dpath, "/");
			strcat(dpath, de[i]->d_name);

			if (strcmp(hex, hexname) == 0)
				enqueue(q, dpath);
			free(hex);
		}
	}

	for (int i=0;i<dirnum;i++)
		free(de[i]);
	free(de);
}

