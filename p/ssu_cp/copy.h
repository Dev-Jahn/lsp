#ifndef _COPY_H
#define _COPY_H

void print_stat(const char *src);
void copy_file(const char *src, const char *tgt);
void copy_dir(const char *src, const char *tgt);
int filter(const struct dirent *dir);
void recur (const char *src, const char *tgt);
void proc(const char *src, const char *tgt);
void copy_stat(const char *src, const char *tgt);
void print_stat(const char *src);

#endif
