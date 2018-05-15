#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <libgen.h>
#include "data.h"
#include "error.h"

BakEntry *add(BakTable *table, char *abspath)
{
	struct stat statbuf;
	BakEntry *e = (BakEntry*)malloc(sizeof(BakEntry));
	if (stat(abspath, &statbuf)<0)
		error(STAT, abspath);
	strcpy(e->filename, basename(abspath));
	strcpy(e->abspath, abspath);
	e->mode = statbuf.st_mode;
	e->size = statbuf.st_size;
	e->mtime_last = statbuf.st_mtime;


		

	return e;
}
BakEntry *search(BakTable *table, const char *abspath);
/*check if the file has been modified*/
int check_modified(const char *abspath);
int delete_old(const char *abspath);
void compare(void);
void restore(void);
