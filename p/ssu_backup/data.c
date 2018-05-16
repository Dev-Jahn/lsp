#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <libgen.h>
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
	sha256_file(abspath, e->checksum_last);
#endif
	e.filecnt = 1;

	memcpy(&(table->be[table->cnt]), &e, sizeof(BakEntry));
	table->cnt++;
	return &(table->be[table->cnt]);
}
BakEntry *renew_bak(BakTable *table, const char *abspath)
{
		
}
int remove_bak(BakTable *table, const char *abspath);
BakEntry *search_bak(BakTable *table, const char *abspath)
{
	BakEntry *e = NULL;
	return e;
}
/*check if the file has been modified*/
int check_modified(const char *abspath);
int delete_old(const char *abspath);
void compare_bak(void);
void restore_bak(void);
