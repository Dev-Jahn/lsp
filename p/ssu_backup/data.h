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
void init_table(BakTable *table);
BakEntry *add_bak(BakTable *table, const char *abspath);
BakEntry *renew_bak(BakTable *table, const char *abspath);
int remove_bak(BakTable *table, const char *abspath);
BakEntry *search_bak(BakTable *table, const char *abspath);
int check_modified(const char *abspath);
int delete_old(const char *abspath);
void compare_bak(void);
void restore_bak(void);

#endif
