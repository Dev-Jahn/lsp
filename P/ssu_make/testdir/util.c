#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "util.h"
#include "struct.h"
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
		exit(1);
	}
	if ((err = regexec(&regex, string, 1, pmatch, 0)) != 0)
	{
		regerror(err, &regex, errbuf, sizeof(errbuf));
		fprintf(stderr, "Match failed: %s\n",errbuf);
		regfree(&regex);
		if (err == REG_NOMATCH)
			return NULL;
		else
			exit(1);
	}
	else
	{
		int nbytes = pmatch[0].rm_eo - pmatch[0].rm_so;
		memcpy(substring, string, nbytes);
		substring[nbytes] = 0;
	}
	return substring;	
}
/*
 * @param filedescriptor
 * @return offset struct containing start, end offset and found flag
 * 파일에서 패턴에 맞는 스트링의 첫번째 매칭 오프셋 리턴
 */
Off_Pair regfind(int fd, const char *pattern)
{
	Off_Pair off_p;
	off_p.found = 0;
	regex_t regex;
	regmatch_t pmatch[1];
	int err;
	char errbuf[100];
	int blk_size = 1, line_size = 0;
	char buf[CHAR_PER_LINE];
	
	if ((err = regcomp(&regex, pattern, REG_EXTENDED)) != 0)
	{
		regerror(err, &regex, errbuf, sizeof(errbuf));
		fprintf(stderr,"Could not compile regex\n");
		regfree(&regex);
		exit(1);
	}
	//매칭문자열을 찾을때까지 반복
	while (blk_size - line_size)
	{
		blk_size = read(fd,buf,CHAR_PER_LINE); 	
		for (int i=0; i<blk_size; i++)
		{
			if (buf[i]=='\n')
			{
				buf[i+1]=0;
				line_size = i+1;
				break;
			}
		}
		//라인에서 일치항을 찾았을때
		if ((err = regexec(&regex, buf, 1, pmatch, 0)) == 0)
		{
			off_p.so = lseek(fd, 0, SEEK_CUR) - blk_size +(off_t)pmatch[0].rm_so;
			off_p.eo = off_p.so - (off_t)pmatch[0].rm_so + (off_t)pmatch[0].rm_eo;
			off_p.found = 1;
			break;
		}
		//일치항이 없을때
		else
		{
			off_p.so = (off_t)0;
			off_p.eo = (off_t)0;
			//불일치 외의 에러일때 에러 출력
			if (err != REG_NOMATCH)
			{
				regerror(err, &regex, errbuf, sizeof(errbuf));
				fprintf(stderr, "Match failed: %s\n",errbuf);
				regfree(&regex);
				exit(1);
			}
		}
		lseek(fd, line_size - blk_size, SEEK_CUR);
	}
	lseek(fd, off_p.eo, SEEK_SET);
	return off_p;
}
