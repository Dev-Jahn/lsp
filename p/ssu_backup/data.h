#ifndef _DATA_H
#define _DATA_H

#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <openssl/sha.h>

typedef struct _BakEntry
{
	char filename[NAME_MAX];
	char abspath[PATH_MAX];
	time_t mtime_last;	
	char checksum_last[SHA256_DIGEST_LENGTH*2 + 1];
	char oldest[PATH_MAX];
	size_t filecnt;
} BakEntry;

typedef struct _BakTable
{
	BakEntry *be;
	size_t cnt;
} BakTable;

int add(BakTable *table, BakEntry entry);
BakEntry *search(BakTable *table, const char *abspath);
int check_modified(const char *abspath);
int delete_old(const char *abspath);
void compare(void);
void restore(void);

#endif
