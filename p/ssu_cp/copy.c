#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utime.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "copy.h"
#include "ssu_cp.h"
#include "error.h"

#define O_OVERWR O_RDWR|O_CREAT|O_TRUNC
#define O_NOVERW O_RDWR|O_CREAT|O_EXCL
#define S_MODE 0644
#define BUFSIZE 1024

void copy(const char *src, const char *tgt)
{
	if (ON_S(flag))
	{
		if (symlink(src, tgt)<0)
			error(SYM,src);
		return;
	}
	if (ON_P(flag))
		print_stat(src);
	if (ON_R(flag))
		;
	else if (ON_D(flag))
		;
	else
	




	if (ON_L(flag))
		;
	
}

void copy_file(const char *src, const char *tgt)
{
	int fd1, fd2, len;
	char buf[BUFSIZE];
	if ((fd1 = open(src, O_RDONLY))<0)
		error(OPEN, src);
	if (ON_N(flag))
	{
		if ((fd2 = open(tgt, O_NOVERW))<0)
		{
			printf("<%s> already exist.", tgt);
			exit(1);
		}
	}
	else if ((fd2 = open(tgt, O_OVERWR))<0)
		error(OPEN, tgt);
	while((len = read(fd1, buf, BUFSIZE))>0)
		write(fd2, buf, len);
}

void copy_dir(const char *src, const char *tgt)
{
	
	
}

void copy_stat(const char *src, const char *tgt)
{
	struct stat statbuf;
	struct utimbuf timebuf;
	if (stat(src, &statbuf)<0)
		error(STAT, src);
	if (chmod(tgt, statbuf.st_mode)<0)
		error(CHMOD, tgt);
	if (chown(tgt, statbuf.st_uid, statbuf.st_gid)<0)
		error(CHOWN, tgt);
	timebuf.actime = statbuf.st_atime;
	timebuf.modtime = statbuf.st_mtime;
	if (utime(tgt,&timebuf)<0)
		error(UTIME, tgt);
}

void print_stat(const char *src)
{
	struct stat statbuf;
	struct tm *atime_p, *mtime_p, *ctime_p;
	struct passwd *usr;
	struct group *grp;
	if (stat(src, &statbuf)<0)
		error(STAT, src);
	atime_p = localtime(&statbuf.st_atime);
	mtime_p = localtime(&statbuf.st_mtime);
	ctime_p = localtime(&statbuf.st_ctime);
	usr = getpwuid(statbuf.st_uid);
	grp = getgrgid(statbuf.st_gid);

	printf("Filename : %s\n",src);
	printf("Last access time : %d.%d.%d %d:%d:%d\n",atime_p->tm_year+1900, atime_p->tm_mon+1, atime_p->tm_mday, atime_p->tm_hour, atime_p->tm_min, atime_p->tm_sec);
	printf("Last modified time : %d.%d.%d %d:%d:%d\n",mtime_p->tm_year+1900, mtime_p->tm_mon+1, mtime_p->tm_mday, mtime_p->tm_hour, mtime_p->tm_min, mtime_p->tm_sec);
	printf("Last changed time : %d.%d.%d %d:%d:%d\n",ctime_p->tm_year+1900, ctime_p->tm_mon+1, ctime_p->tm_mday, ctime_p->tm_hour, ctime_p->tm_min, ctime_p->tm_sec);
	printf("Owner : %s\n", usr->pw_name);
	printf("Group : %s\n", grp->gr_name);
	printf("File size : %ld Bytes\n", statbuf.st_size);

}
