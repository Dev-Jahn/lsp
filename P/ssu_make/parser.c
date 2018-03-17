#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "parser.h"

COMMAND *parse_cmd(int argc, char *argv[])
{
	COMMAND *cmd = (COMMAND*)malloc(sizeof(COMMAND));
	STACK tokenstk;
	tokenstk.size = 0;
	for (int i=1;i<argc;i++)
	{
		push(&tokenstk, argv[i]);
	}

	regex_t regex_opt, regex_mac;
	int cnt_opt=0, cnt_tar=0, cnt_mac=0;

	//옵션지정토큰 1차 필터링 패턴
	char *pat_opt1 = "-\\w+";
	//적합한 단일옵션토큰패턴
	char *pat_opt2 = "-[mfchst]";
	//적합한 다중옵션토큰패턴
	char *pat_opt3 = "-[mhst]{2,4}";
	//적합한 매크로토큰패턴
	char *pat_mac = "\\w+=\\w+|\\w+=\"\\w+\"";
	char *pat_mac_l = "\\w+=";
	char *pat_mac_r = "=\\w+";
	//타겟 패턴
	char *pat_tar = "\\w+";
	for (int i=1;i<argc;i++)
	{
		char *token = argv[i];
		//매크로토큰 검출
		if (compare(pat_mac,token))
		{
			if (cnt_mac >= 5)
			{
				/*
				 * 매크로갯수 초과시 예외처리
				 */
				break;
			}
			char *k = trim(pat_mac_l, token);
			k = trim("\\w+", k);
			char *v = trim(pat_mac_r, token);
			v = trim("\\w+", v);

			cmd->macro[cnt_mac].key = k;
			cmd->macro[cnt_mac].value = v;
			cnt_mac++;
		}
		else if (compare(pat_

	}
	
	return cmd;
}
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
