#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "error.h"

void error(enum ErrCode err, const char *arg)
{
	switch (err)
	{
		case REOPT:
			fprintf(stderr, "Redundant option '-%s' detected.\n", arg);
			break;
		case COLOPT:
			fprintf(stderr, "Those options cannot be used together.\n");
			break;
		case SONLY:
			fprintf(stderr, "Option '-s' cannot be used with other options.\n");
			break;
		case LIMIT:
			fprintf(stderr, "Length of the path exceeded limit.\nPath should be less than %d Bytes.\n", PATH_MAX);
			break;
		case USAGE:
			fprintf(stderr, "Usage: ssu_cp [OPTION] [SOURCE] [TARGET]\n");
			break;
		case MISSING:
			fprintf(stderr, "Source file '%s' does not exist.\n", arg);
			break;
		case SAME:
			fprintf(stderr, "Source and target file is same.\n");
			break;
		case ISDIR:
			fprintf(stderr, "'%s' is a directory. Use -r or -d option.\n", arg);
			break;
		case OPEN:
			fprintf(stderr, "open error: '%s'.\n", arg);
			break;
		case STAT:
			fprintf(stderr, "stat error: '%s'.\n", arg);
			break;
		case CHMOD:
			fprintf(stderr, "chmod error: '%s'.\n", arg);
			break;
		case CHOWN:
			fprintf(stderr, "chown error: '%s'.\n", arg);
			break;
		case UTIME:
			fprintf(stderr, "utime error: '%s'.\n", arg);
			break;
		case SYM:
			fprintf(stderr, "symlink error: '%s'.\n", arg);
			break;
		case SCAN:
			fprintf(stderr, "scandir error: '%s'.\n", arg);
			break;
		case MKDIR:
			fprintf(stderr, "mkdir error: '%s'.\n", arg);
			break;
		case ARGD:
			fprintf(stderr, "'%s' is not a proper argument for option '-d'.\nUse integer between 1 to 9.\n", arg);
			break;
		default:
			fprintf(stderr, "Unknown error.\n");
	}
	exit(1);
}
