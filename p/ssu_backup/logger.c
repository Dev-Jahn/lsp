#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <syslog.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include "ssu_backup.h"
#include "logger.h"
#include "data.h"
#include "util.h"

#include <errno.h>

char logdirpath[PATH_MAX] = "log";		/*log directory path*/
char logpath[PATH_MAX];

void log_init(void)
{
	char abspath[PATH_MAX] = {0};
	struct stat dirstat;
	realpath(logdirpath, abspath);
	strcpy(logdirpath, abspath);
	if (stat(logdirpath, &dirstat) < 0)
	{
		if (mkdir(logdirpath, 0777) < 0)
			error(MKDIR, logdirpath);
	}
	else if (!S_ISDIR(dirstat.st_mode))
		error(MKDIR, logdirpath);

	strcpy(logpath, logdirpath);
	strcat(logpath, "/");
	strcat(logpath, "log.txt");
}

void baklog(enum State st, BakEntry *bak, ...)
{
	va_list ap;
	va_start(ap, bak);

	if (st == START)
		filelog("===========================================\n");

	/*Timestamp of logging time*/
	char stamp[16] = {0};
	timestamp(time(NULL), stamp, sizeof(stamp), "%m%d %H:%M:%S");
	filelog("[%s] ", stamp);

	struct stat bakstat;
	switch (st)
	{
	case START:
		filelog( "ssu_backup <pid:%d> Start\n", getpid());
		break;
	case INIT:
		filelog("Backup daemon initialized.\n");
		break;
	case BACKUP:
		if (stat((char*)(&bak->abspath),&bakstat)<0)
			error(STAT, &bak->abspath);
		timestamp(bak->mtime_last, stamp, sizeof(stamp), "%m%d %H:%M:%S");
		filelog("%s [size:%ld/mtime:%s]\n",bak->filename, bakstat.st_size, stamp);
		break;
	case MODIFIED:
		if (stat((char*)(&bak->abspath),&bakstat)<0)
			error(STAT, &bak->abspath);
		timestamp(bak->mtime_last, stamp, sizeof(stamp), "%m%d %H:%M:%S");
		filelog("%s is modified. [size:%ld/mtime:%s]\n",bak->filename, bakstat.st_size, stamp);
		break;
	case ADDED:
		{
			char *file = va_arg(ap, char*);
			char *dir = va_arg(ap, char*);
			filelog("New file %s is created in %s.\n", file, dir);
		}
		break;
	case DELFILE:
		filelog("%s is deleted.\n",bak->filename);
		break;
	case DELDIR:
		filelog("%s is deleted.\n",va_arg(ap, char*));
		break;
	case DELOLD:
		if (stat((char*)peek(&bak->fileQue),&bakstat)<0)
			error(STAT, &bak->fileQue);
		timestamp(bakstat.st_mtime, stamp, sizeof(stamp), "%m%d %H:%M:%S");
		filelog("Delete oldest [%s, size:%ld, btime:%s]\n",bak->filename, bakstat.st_size, stamp);
		break;
	case DIFF:
		{
			char *a = va_arg(ap, char*);
			char *b = va_arg(ap, char*);
			filelog("Compared [%s] with [%s].\n", a, b);
		}
		break;
	case RESTORE:
		{
			char *a = va_arg(ap, char*);
			char *b = va_arg(ap, char*);
			filelog("Restored [%s] from [%s].\n", a, b);
		}
		break;
	case SIGNAL1:
		filelog("Received SIGUSR1 from <pid:%d>. Discard current entries.\n", va_arg(ap, pid_t));
		break;
	case SIGNAL2:
		filelog("Received SIGUSR2 from <pid:%d>. Transfer current entries.\n", va_arg(ap, pid_t));
		break;
	case EXIT:
		filelog("ssu_backup <pid:%d> Exit\n", getpid());
		break;
	}
	va_end(ap);	
}

void filelog(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	FILE *fp_log;
	if ((fp_log = fopen(logpath, "a")) == NULL)
	{
		errlog("%s",strerror(errno));
		error(OPEN, logpath);
	}
	vfprintf(fp_log, format, ap);
	fclose(fp_log);

	va_end(ap);
}
/* ---------------------------------*/
/**
 * @brief 
 *
 * @param format Format string for logging
 * @param ... Argument for additional information
 */
/* ---------------------------------*/
void errlog(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);

	openlog("ssu_backup", LOG_PID|LOG_PERROR, 0);
	vsyslog(LOG_ERR, format, ap);
	closelog();

	va_end(ap);
}

/*
 *처리안한 에러
 *SAME
 *ONFILE
 */

/* ---------------------------------*/
/**
 * @brief Comprehensive error processing.
 *
 * @param err Error code. See header file for more info
 * @param ... Argument for additional information
 */
/* ---------------------------------*/
void error(enum ErrCode err, ...)
{
	va_list ap;
	va_start(ap, err);

	switch (err)
	{
	case NAMELIM:
		errlog("Filename is too long. [Limit : %d bytes]\n", NAME_MAX);
		break;
	case PATHLIM:
		errlog("Pathname is too long. [Limit : %d bytes]\n", PATH_MAX);
		break;
	case NAOPT:
		errlog("'%s' is undefined option. Please check the help page. 'ssu_backup -h'\n", va_arg(ap, char*));
		break;
	case RONLY:
		errlog("Option 'r' cannot be used with other options.\n");
		break;
	case LESSARG:
		errlog("Need more arguments.\n");
		break;
	case MOREARG:
		errlog("Too much arguments. No period is required in -r, -c.\n");
		break;
	case NAPRD:
		errlog("Period must be between 3 and 10.\n");
		break;
	case NOFILE:
		errlog("There is no file named '%s'.\n", va_arg(ap, char*));
		break;
	case NEEDD:
		errlog("'%s' is directory. Use '-d' option.\n", va_arg(ap, char*));
		break;
	case NOTDIR:
		errlog("'%s' is not a directory.\n", va_arg(ap, char*));
		break;
	case NOTREG:
		errlog("'%s' is not a regular file.\n", va_arg(ap, char*));
		break;
	case MKDIR:
		errlog("mkdir error for '%s'.\n", va_arg(ap, char*));
		break;
	case OPEN:
		errlog("open error for '%s'.\n", va_arg(ap, char*));
		break;
	case STAT:
		errlog("stat error for '%s'.\n", va_arg(ap, char*));
		break;
	case REMOVE:
		errlog("remove error for '%s'.\n", va_arg(ap, char*));
		break;
	case SCAN:
		errlog("scandir error for '%s'.\n", va_arg(ap, char*));
		break;
	case CHMOD:
		errlog("chmod error for '%s'.\n", va_arg(ap, char*));
		break;
	case PTHCREAT:
		errlog("pthread_create error.\n");
		break;
	default:
		errlog("Unknown error.[%d]\n", err);
	}

	va_end(ap);
	exit(1);
}
