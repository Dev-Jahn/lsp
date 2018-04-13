#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <regex.h>
#include "util.h"

/* ---------------------------------*/
/**
 * @brief 입력한 regex패턴과 문자열의 매칭을 확인하는 함수
 *
 * @param pattern POSIX Extended 문법을 따르는 정규표현식 문자열
 * @param string 매칭시킬 타겟 문자열
 *
 * @return 매칭 실패시 -1, 완전일치시 0, 포함시 1
 */
/* ---------------------------------*/
int compare(const char *pattern, const char *string)
{
	regex_t regex;
	int err;
	char errbuf[100];
	regmatch_t pmatch[1];

	if ((err = regcomp(&regex, pattern, REG_EXTENDED|REG_NEWLINE)) != 0)
	{
		regerror(err, &regex, errbuf, sizeof(errbuf));
		fprintf(stderr,"Error: Could not compile regex\n");
		regfree(&regex);
		return -1;
	}
	if ((err = regexec(&regex, string, 1, pmatch, 0)) != 0)
	{
		if (err != REG_NOMATCH)
		{
			regerror(err, &regex, errbuf, sizeof(errbuf));
			fprintf(stderr, "Error: Match failed: %s\t(in compare)\n",errbuf);
		}
		regfree(&regex);
		return -1;
	}
	else if ((int)strlen(string) == pmatch[0].rm_eo - pmatch[0].rm_so)
	{
		regfree(&regex);
		return 0;
	}
	else
		return 1;
}
