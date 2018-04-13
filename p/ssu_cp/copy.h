#ifndef _COPY_H
#define _COPY_H

#include <dirent.h>

void copy(const char *src, const char *tgt);
void copy_file(const char *src, const char *tgt);
int filter(const struct dirent *dir);
void copy_dir(const char *src, const char *tgt);
void copy_proc(const char *src, const char *tgt);
void copy_stat(const char *src, const char *tgt);
void copy_mode(const char *src, const char *tgt);
void print_stat(const char *src);

#endif
