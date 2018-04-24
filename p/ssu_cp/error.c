#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "error.h"

/* ---------------------------------*/
/**
 * @brief 에러 일괄처리
 *
 * @param err 에러코드, 헤더참조
 * @param arg 추가정보를 위한 인자
 */
/* ---------------------------------*/
void error(enum ErrCode err, const char *arg)
{
	switch (err)
	{
		case REOPT:
			fprintf(stderr, "Redundant option '%s' detected.\n", arg);
			break;
		case COLOPT:
			fprintf(stderr, "Option '%s' collides with preceding options.\n", arg);
			break;
		case SONLY:
			fprintf(stderr, "Option '-s' cannot be used with other options.\n");
			break;
		case LIMIT:
			fprintf(stderr, "Length of the path exceeded limit.\nPath should be less than %d Bytes.\n", PATH_MAX);
			break;
		case USAGE:
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
		case NOTDIR:
			fprintf(stderr, "'%s' is not a directory. -r or -d is only for directory copy.\n", arg);
			break;
		case ONFILE:
			fprintf(stderr, "File '%s' already exist. Can't overwrite with directory.\n", arg);
			break;
		case OPEN:
			fprintf(stderr, "open error: '%s'.\n", arg);
			break;
		case NOFILE:
			fprintf(stderr, "There's no such file or directory.'%s'\n", arg);
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
			fprintf(stderr, "'%s' is not a proper argument for option '-d'.\nUse integer between 1 and 10.\n", arg);
			break;
		default:
			fprintf(stderr, "Unknown error.\n");
	}
	fprintf(stderr, "\
Usage: ssu_cp [OPTION] [SOURCE] [TARGET]\n\
(file)\n\
	ssu_cp [-i/n][-l][-p]	[srcfile] [tgtfile]\n\
	ssu_cp [-s]	[srcfile] [tgtfile]\n\
(directory)\n\
	ssu_cp [-i/n][-l][-p][-r][-d][N]	[srcdir] [tgtdir]\n");
	exit(1);
}
