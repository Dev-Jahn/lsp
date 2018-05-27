#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <utime.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <string.h>
#include "io.h"
#include "ssu_backup.h"
#include "logger.h"

#define O_OVERWR O_RDWR|O_CREAT|O_TRUNC
#define O_NOVERW O_RDWR|O_CREAT|O_EXCL
#define BUFSIZE 1024

/* ---------------------------------*/
/**
 * @brief copy src file to dst location
 *
 * @param src source
 * @param dst destination
 */
/* ---------------------------------*/
void copy(const char *src, const char *dst)
{
	int fd1, fd2, len;
	char buf[BUFSIZE];
	if ((fd1 = open(src, O_RDONLY))<0)
		error(OPEN, src);
	if ((fd2 = open(dst, O_WRONLY|O_CREAT|O_TRUNC, 0666))<0)
		error(OPEN, dst);
	while((len = read(fd1, buf, BUFSIZE))>0)
		write(fd2, buf, len);
	close(fd1);
	close(fd2);
}

/* ---------------------------------*/
/**
 * @brief copy the mode of the src to dst
 *
 * @param src source
 * @param dst destination
 */
/* ---------------------------------*/
void copy_mode(const char *src, const char *dst)
{
	struct stat statbuf;
	if (stat(src, &statbuf)<0)
		error(NOFILE, src);
	if (chmod(dst, statbuf.st_mode)<0)
		error(CHMOD, dst);
}
