#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "util.h"
#define MATCH_MIN 10

/*
 * 패턴컴파일 오류, 매칭 실패 시 -1
 * 매칭성공, 패턴 전체와 불일치시 1
 * 패턴과 완전 일치시 0
 */
int compare(const char *pattern, const char *string)
{
	regex_t regex;
	int err;
	char errbuf[100];
	regmatch_t pmatch[1];

	if ((err = regcomp(&regex, pattern, REG_EXTENDED)) != 0)
	{
		regerror(err, &regex, errbuf, sizeof(errbuf));
		fprintf(stderr,"Could not compile regex\n");
		regfree(&regex);
		return -1;
	}
	if ((err = regexec(&regex, string, 1, pmatch, 0)) != 0)
	{
		regerror(err, &regex, errbuf, sizeof(errbuf));
		fprintf(stderr, "Match failed: %s\n",errbuf);
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

char *trim(const char *pattern, const char *string)
{
	char *substring = (char*)malloc(sizeof(char));
	int err = 0;
	char errbuf[100];
	regex_t regex;
	regmatch_t pmatch[1];

	if ((err = regcomp(&regex, pattern, REG_EXTENDED)) != 0)
	{
		regerror(err, &regex, errbuf, sizeof(errbuf));
		fprintf(stderr,"Could not compile regex\n");
		regfree(&regex);
		return 0;
	}
	if ((err = regexec(&regex, string, 1, pmatch, 0)) != 0)
	{
		regerror(err, &regex, errbuf, sizeof(errbuf));
		fprintf(stderr, "Match failed: %s\n",errbuf);
		regfree(&regex);
		return 0;
	}
	else
	{
		int nbytes = pmatch[0].rm_eo - pmatch[0].rm_so;
		memcpy(substring, string, nbytes);
		substring[nbytes] = 0;
	}
	return substring;	
}
