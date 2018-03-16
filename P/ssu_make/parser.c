#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "parser.h"

COMMAND *parse_cmd(int argc, char *argv[])
{
	COMMAND *cmd = (COMMAND*)malloc(sizeof(COMMAND));

	regex_t regex_opt, regex_mac;
	int cnt_opt=0, cnt_tar=0, cnt_mac=0;

	//옵션지정토큰 1차 필터링 패턴
	char *pat_opt1 = "-\\w*";
	//적합한 단일옵션토큰패턴
	char *pat_opt2 = "-[mfchst]";
	//적합한 다중옵션토큰패턴
	char *pat_opt3 = "-[mhst]{2,4}";
	//적합한 매크로토큰패턴
	char *pat_mac = "\\w+=\\w+|\\w+=\"\\w+\"";
	//타겟 패턴
	char *pat_tar = "\\w+";
	for (int i=1;i<argc;i++)
	{
		char *token = argv[i];
		//매크로토큰 검출
		if (compare(pat_mac,token))
		{
			cnt_mac++;
			char *k;
			char *v;
			
		}

	}
	
	return cmd;
}

int compare(const char *pattern, const char *string)
{
	regex_t regex;
	int err;
	char errbuf[100];

	if ((err = regcomp(&regex, pattern, REG_EXTENDED)) != 0)
	{
		regerror(err, &regex, errbuf, sizeof(errbuf));
		fprintf(stderr,"Could not compile regex\n");
		regfree(&regex);
		exit(1);
	}
	if ((err = regexec(&regex, string, 0, NULL, 0)) != 0)
	{
		regerror(err, &regex, errbuf, sizeof(errbuf));
		fprintf(stderr, "Match failed: %s\n",errbuf);
		regfree(&regex);
		exit(1);
	}
	else
	{
		regfree(&regex);
		return 0;
	}
}

char *trim(const char *pattern, const char *string)
{
	char *substring = (char*)malloc(sizeof(char));
	
	
}
