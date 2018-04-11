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
#include <dirent.h>
#include <string.h>
#include "copy.h"
#include "ssu_cp.h"
#include "error.h"

#define O_OVERWR O_RDWR|O_CREAT|O_TRUNC
#define O_NOVERW O_RDWR|O_CREAT|O_EXCL
#define S_FILE 0644
#define S_DIR 0755
#define BUFSIZE 1024

char path[PATH_MAX];

void copy(const char *src, const char *tgt)
{
	//상대, 절대경로 혼용할때도 예외처리 가능하도록 수정할것
	if (strcmp(src, tgt) == 0)
		error(SAME,src);
	struct stat statbuf;
	if (stat(src, &statbuf)<0)
		error(STAT, src);
	if (ON_S(flag))
	{
		if (symlink(src, tgt)<0)
			error(SYM,src);
		return;
	}
	if (ON_P(flag))
		print_stat(src);

	if (S_ISDIR(statbuf.st_mode))
	{
		if (ON_R(flag))
			recur(src, tgt);
		else if (ON_D(flag))
			proc(src, tgt);
		else
			error(ISDIR, src);
	}
	




	if (ON_L(flag))
		copy_stat(src, tgt);
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
	if (mkdir(tgt, S_DIR)<0)
		error(MKDIR, tgt);


}
int filter(const struct dirent *dir)
{
	if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
		return 0;
	else
		return 1;
}

/* ---------------------------------*/
/**
 * @brief Use only for directory
 *
 * @param src
 * @param tgt
 */
/* ---------------------------------*/
void recur(const char *src, const char *tgt)
{
	struct dirent **namelist;
	int entry;
	if (mkdir(tgt, S_DIR)<0)
		error(MKDIR, tgt);
	if ((entry = scandir(src, &namelist, filter, alphasort))<0)
		error(SCAN, src);
	else
	{
		for (int i=0;i<entry;i++)
		{
			if (namelist[i]->d_type == DT_DIR)
			{
				recur(namelist[i]->d_name,
			}
			else
			{
				copy_file(namelist[i]->d_name,
			}
		}
	}





	//메모리 할당 해제
	for (int i=0;i<entry;i++)
		free(namelist[i]);
	free(namelist);
}

void proc(const char *src, const char *tgt)
{
	struct dirent **namelist;
	int entry;
	if ((entry = scandir(src, &namelist, filter, alphasort))<0)
		error(SCAN, src);
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
