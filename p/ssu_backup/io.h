#ifndef _IO_H
#define _IO_H

#include <dirent.h>

void copy(const char *src, const char *tgt);
void copy_file(const char *src, const char *tgt);
void copy_dir(const char *src, const char *tgt);
void copy_mode(const char *src, const char *tgt);

//미구현
void cat(void);

#endif
