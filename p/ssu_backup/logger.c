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
#include <signal.h>
#include <time.h>
#include <errno.h>
#include "ssu_backup.h"
#include "logger.h"
#include "data.h"
#include "util.h"

char logdirpath[PATH_MAX] = "log";		/*log directory path*/
char logpath[PATH_MAX];

/*Internal usage*/
static void filelog(const char *, ...);

/* ---------------------------------*/
/**
 * @brief Create log directory and path
 */
/* ---------------------------------*/
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

/* ---------------------------------*/
/**
 * @brief Print the log to file for corresponding situation.
 *
 * @param st Current state
 * @param bak Entry data of backup
 * @param ... Additional arguments
 */
/* ---------------------------------*/
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
		filelog("Backup daemon <pid:%d> initialized.\n", getpid());
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

/* ---------------------------------*/
/**
 * @brief Print formatted string to log file
 *
 * @param format Format string
 * @param ... Additional arguments
 */
/* ---------------------------------*/
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
 * @brief Print log to syslog and stderr
 *
 * @param format Format string
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

/* ---------------------------------*/
/**
 * @brief Comprehensive error processing.
 *
 * @param err Error code. See header file for more info
 * @param ... Arguments for additional information
 */
/* ---------------------------------*/
void error(enum ErrCode err, ...)
{
	va_list ap;
	va_start(ap, err);

	char errstr[1024];
	char substr[1024];
	strcpy(errstr, "ERROR: ");
	switch (err)
	{
	case NAMELIM:
		strcpy(substr,
			"Filename is too long. [Limit : %d bytes]\n");
		strcat(errstr,substr);
		errlog(errstr, NAME_MAX);
		break;
	case PATHLIM:
		strcpy(substr,
			"Pathname is too long. [Limit : %d bytes]\n");
		strcat(errstr,substr);
		errlog(errstr, PATH_MAX);
		break;
	case NAOPT:
		strcpy(substr,
			"'%s' is undefined option. Please check the help page. 'ssu_backup -h'\n");
		strcat(errstr,substr);
		errlog(errstr, va_arg(ap, char*));
		break;
	case RONLY:
		strcpy(substr,
			"Option 'r' cannot be used with other options.\n");
		strcat(errstr,substr);
		errlog(errstr);
		
		break;
	case LESSARG:
		strcpy(substr,
			"Need more arguments.\n");
		strcat(errstr,substr);
		errlog(errstr);
		usage();
		break;
	case MOREARG:
		strcpy(substr,
			"Too much arguments. No period is required in -r, -c.\n");
		strcat(errstr,substr);
		errlog(errstr);
		usage();
		break;
	case NAPRD:
		strcpy(substr,
			"Period must be between 3 and 10.\n");
		strcat(errstr,substr);
		errlog(errstr);
		break;
	case NACNT:
		strcpy(substr,
			"Argument of '-n' must be natural number.\n");
		strcat(errstr,substr);
		errlog(errstr);
		break;
	case NOFILE:
		strcpy(substr,
			"There is no file named '%s'.\n");
		strcat(errstr,substr);
		errlog(errstr, va_arg(ap, char*));
		break;
	case NEEDD:
		strcpy(substr,
			"'%s' is directory. Use '-d' option.\n");
		strcat(errstr,substr);
		errlog(errstr, va_arg(ap, char*));
		break;
	case NOTDIR:
		strcpy(substr,
			"'%s' is not a directory.\n");
		strcat(errstr,substr);
		errlog(errstr, va_arg(ap, char*));
		break;
	case NOTREG:
		strcpy(substr,
			"'%s' is not a regular file.\n");
		strcat(errstr,substr);
		errlog(errstr, va_arg(ap, char*));
		break;
	case MKDIR:
		strcpy(substr,
			"mkdir error for '%s'.\n");
		strcat(errstr,substr);
		errlog(errstr, va_arg(ap, char*));
		break;
	case OPEN:
		strcpy(substr,
			"open error for '%s'.\n");
		strcat(errstr,substr);
		errlog(errstr, va_arg(ap, char*));
		break;
	case STAT:
		strcpy(substr,
			"stat error for '%s'.\n");
		strcat(errstr,substr);
		errlog(errstr, va_arg(ap, char*));
		break;
	case REMOVE:
		strcpy(substr,
			"remove error for '%s'.\n");
		strcat(errstr,substr);
		errlog(errstr, va_arg(ap, char*));
		break;
	case SCAN:
		strcpy(substr,
			"scandir error for '%s'.\n");
		strcat(errstr,substr);
		errlog(errstr, va_arg(ap, char*));
		break;
	case CHMOD:
		strcpy(substr,
			"chmod error for '%s'.\n");
		strcat(errstr,substr);
		errlog(errstr, va_arg(ap, char*));
		break;
	case PTHCREAT:
		strcpy(substr,
			"pthread_create error.\n");
		strcat(errstr,substr);
		errlog(errstr);
		break;
	case RREAL:
		strcpy(substr,
			"When restoring non-existing file, use absolute path.\n");
		strcat(errstr,substr);
		errlog(errstr);
		break;
	default:
		strcpy(substr,
			"Unknown error.[%d]\n");
		strcat(errstr,substr);
		errlog(errstr, err);
	}

	va_end(ap);
	/*If the process is daemon, call signal handler*/
	if (getpgrp() == getsid(0))
		raise(SIGABRT);
	else
		exit(1);
}
