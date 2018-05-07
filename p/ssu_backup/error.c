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
		case NAMEMAX:
			fprintf(stderr, "Length of the filename exceeded the limit.(%d bytes)\n", NAME_MAX);
			break;
		case PATHMAX:
			fprintf(stderr, "Length of the path exceeded the limit.(%d bytes)\n", PATH_MAX);
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
