#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>
#include "error.h"
#include "logger.h"

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
		errlog("Too much arguments.\n");
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
	case SCAN:
		errlog("scandir error for '%s'.\n", va_arg(ap, char*));
		break;
	case CHMOD:
		errlog("chmod error for '%s'.\n", va_arg(ap, char*));
		break;
	default:
		errlog("Unknown error.[%d]\n", err);
	}

	va_end(ap);
	exit(1);
}
