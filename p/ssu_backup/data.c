#include "data.h"

int add(BakTable *table, BakEntry entry);
BakEntry *search(BakTable *table, const char *abspath);
/*check if the file has modified*/
int check_modified(const char *abspath);
int delete_old(const char *abspath);
void compare(void);
void restore(void);
