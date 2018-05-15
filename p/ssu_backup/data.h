#ifndef _DATA_H
#define _DATA_H

#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#ifdef SHA
#include <openssl/sha.h>
#endif

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
	char oldest[PATH_MAX];
	size_t filecnt;
} BakEntry;

typedef struct _BakTable
{
	BakEntry *be;
	size_t cnt;
} BakTable;

BakEntry *add(BakTable *table, char *abspath);
int remove(BakTable *table, const char *abspath);
BakEntry *search(BakTable *table, const char *abspath);
int check_modified(const char *abspath);
int delete_old(const char *abspath);
void compare(void);
void restore(void);

#endif
