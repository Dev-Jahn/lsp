#include <syslog.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>
#include "ssu_backup.h"

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

void baklog(const char *format, ...)
{
	
}
