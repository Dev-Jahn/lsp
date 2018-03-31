#define DEBUGX

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <regex.h>
#include "util.h"
#include "struct.h"
#include "constants.h"
#include "io.h"

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
		fprintf(stderr,"Could not compile regex\n");
		regfree(&regex);
		return -1;
	}
	if ((err = regexec(&regex, string, 1, pmatch, 0)) != 0)
	{
		if (err != REG_NOMATCH)
		{
			regerror(err, &regex, errbuf, sizeof(errbuf));
			fprintf(stderr, "Match failed: %s\t(in compare)\n",errbuf);
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
/* ---------------------------------*/
/**
 * @brief 문자열에서 패턴과 처음 일치하는 부분을 잘라내는 함수
 *
 * @param pattern POSIX Extended 문법을 따르는 정규표현식 문자열
 * @param string 매칭시킬 타겟 문자열
 *
 * @return 패턴과 일치하는 첫번재 문자열을 리턴.사용후 메모리해제 필요. 일치하는 문자열이 없을 시 NULL 리턴.
 */
/* ---------------------------------*/
char *trim(const char *pattern, const char *string)
{
	char *substring = (char*)malloc(strlen(string));
	int err = 0;
	char errbuf[100];
	regex_t regex;
	regmatch_t pmatch[1];

	if ((err = regcomp(&regex, pattern, REG_EXTENDED|REG_NEWLINE)) != 0)
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
		memcpy(substring, string+pmatch[0].rm_so, nbytes);
		substring[nbytes] = '\0';
	}
	return substring;	
}
/* ---------------------------------*/
/**
 * @brief 파일에서 정규표현식패턴을 검색해 처음으로 일치하는 문자열의  시작오프셋, 끝오프셋 쌍을 구하는 함수 
 * 실행 이후 오프셋은 발견한 문자열의 시작오프셋.
 * 발견하지 못하면 SEEK_SET
 *
 * @param fd open된 파일의 descriptor
 * @param pattern 검색할 문자열의 정규표현식 패턴
 *
 * @return 발견여부, 시작오프셋, 끝오프셋을 멤버로 갖는 구조체를 리턴, 찾으면 1,so,eo. 못찾으면 0,0,0.
 */
/* ---------------------------------*/
Off_Pair regfind(int fd, const char *pattern)
{
	Off_Pair off_p;
	off_p.found = 0;
	off_p.so = 0;
	off_p.eo = 0;
	regex_t regex;
	regmatch_t pmatch[1];
	int err;
	ssize_t cnt, sum = 0;
	off_t cur = lseek(fd,0,SEEK_CUR);
	char errbuf[100];
	char buf[BUFFER_SIZE];
	if ((err = regcomp(&regex, pattern, REG_EXTENDED|REG_NEWLINE)) != 0)
	{
		regerror(err, &regex, errbuf, sizeof(errbuf));
		fprintf(stderr,"Could not compile regex\n");
		regfree(&regex);
		exit(1);
	}
	//매칭문자열을 찾을때까지 반복
	while ((cnt = read(fd, buf, BUFFER_SIZE))!=0)
	{
		buf[cnt] = '\0';
		//라인에서 일치항을 찾았을때
		if ((err = regexec(&regex, buf, 1, pmatch, 0)) == 0)
		{
			off_p.so = cur + sum + (off_t)pmatch[0].rm_so;
			off_p.eo = cur + sum + (off_t)pmatch[0].rm_eo;
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
		sum+=cnt;
	}
	lseek(fd, off_p.so, SEEK_SET);
	return off_p;
}

char *sreplace(const char *src, const char *pat, const char *rep)
{
	int err, diff, so = 0, eo = 0, srclen = 0;
	char errbuf[100];
	regex_t regex;
	regmatch_t pmatch[1];
	char *result;
	if (src==NULL||pat==NULL||rep==NULL)
		return NULL;
	if ((err = regcomp(&regex, pat, REG_EXTENDED|REG_NEWLINE)) != 0)
	{
		regerror(err, &regex, errbuf, sizeof(errbuf));
		fprintf(stderr,"Could not compile regex\n");
		regfree(&regex);
		exit(1);
	}
	if ((err = regexec(&regex, src, 1, pmatch, 0)) == 0)
	{
		so = pmatch[0].rm_so;
		eo = pmatch[0].rm_eo;
		srclen = eo-so;
		diff = strlen(rep) - srclen;
		result = (char*)malloc(strlen(src) + diff + 1);
		memcpy(result+so,rep, strlen(rep));
		memcpy(result,src, so);
		memcpy(result+so+strlen(rep),src+eo, strlen(src)-eo);
		result[strlen(src)+diff] = 0;
		return result;
	}
	else
	{
		char *cpy = (char*)malloc(strlen(src)+1);
		memcpy(cpy, src, strlen(src));
		cpy[strlen(src)] = 0;
		return cpy;
	}
}

char *tospace(const char *str)
{
	int len = strlen(str);
	char *ret = (char*)malloc(len+1);
	memset(ret, ' ',len);
	ret[len]='\0';
	return ret;
}
