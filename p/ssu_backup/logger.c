#include <stdio.h>
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
#include "error.h"

char logpath[PATH_MAX];

void log_init(void)
{
	strcpy(logpath, logdirpath);
	strcat(logpath, "/");
	strcat(logpath, "log.txt");
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

void baklog(enum State st, BakEntry *bak)
{
	FILE *fp_log;
	char stamp[16] = {0};
	struct stat bakstat;
	if ((fp_log = fopen(logpath, "a")) == NULL)
		error(OPEN, logpath);
	else
	{
		/*Timestamp of logging time*/
		timestamp(time(NULL), stamp, sizeof(stamp), "%m%d %H:%M:%S");
		fprintf(fp_log, "[%s] ", stamp);

		switch (st)
		{
		case START:
			fprintf(fp_log, "===========================================\n");
			fprintf(fp_log, "ssu_backup <pid:%d> Start\n", getpid());
			break;
		case INIT:
			fprintf(fp_log, "Backup daemon initialized.\n");
			break;
		case BACKUP:
			timestamp(bak->mtime_last, stamp, sizeof(stamp), "%m%d %H:%M:%S");
			fprintf(fp_log, "%s [size:%ld/mtime:%s]\n",bak->filename, bak->size, stamp);
			break;
		case MODIFIED:
			timestamp(bak->mtime_last, stamp, sizeof(stamp), "%m%d %H:%M:%S");
			fprintf(fp_log, "%s is modified. [size:%ld/mtime:%s]\n",bak->filename, bak->size, stamp);
			break;
		case DELETED:
			fprintf(fp_log, "%s is deleted.\n",bak->filename);
			break;
		case DELOLD:
			if (stat((char*)peek(&bak->fileQue),&bakstat)<0)
				error(STAT, &bak->fileQue);
			timestamp(bakstat.st_mtime, stamp, sizeof(stamp), "%m%d %H:%M:%S");
			fprintf(fp_log, "Delete oldest [%s, size:%ld, btime:%s]\n",bak->filename, bakstat.st_size, stamp);
			break;
		case DIFF:
			fprintf(fp_log, "Compared [%s] with [%s].\n", "test", "test");
			break;
		case RESTORE:
			fprintf(fp_log, "Restored [%s] from [%s].\n", "test", "test");
			break;
		case EXIT:
			fprintf(fp_log, "ssu_backup <pid:%d> Exit\n", getpid());
			break;
		}
	}
	
	fclose(fp_log);
}
