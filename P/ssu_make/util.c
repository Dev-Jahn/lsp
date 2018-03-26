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
	char buf[CHAR_PER_LINE];
	if ((err = regcomp(&regex, pattern, REG_EXTENDED)) != 0)
	{
		regerror(err, &regex, errbuf, sizeof(errbuf));
		fprintf(stderr,"Could not compile regex\n");
		regfree(&regex);
		exit(1);
	}
	//매칭문자열을 찾을때까지 반복
	while ((cnt = readLine(fd, buf))!=EOF)
	{
		cnt++;//읽은 바이트수에 개행문자 추가
		//패턴으로 개행문자 인식하려면 별도 스트링 할당해
		//strcat으로 개행 붙이고 처리
		
		//라인에서 일치항을 찾았을때
		if ((err = regexec(&regex, buf, 1, pmatch, 0)) == 0)
		{
			//
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
/* ---------------------------------*/
/**
 * @brief 파일의 현재 오프셋 뒤에 pathname 파일 전체를 삽입 
 *
 * @param fd_dst 붙여넣을 목적지 file descriptor
 * @param pathname 붙여넣을 내용 파일의 경로
 *
 * @return 붙여넣은 총 길이, 원본 파일 오프셋은 붙인내용 뒤로 이동
 */
/* ---------------------------------*/
ssize_t fconcat(int fd_dst, const char *pathname)
{
	
	off_t seek_org = lseek(fd_dst, 0, SEEK_CUR);
	off_t seek_end = lseek(fd_dst, 0, SEEK_END);
	off_t residue = seek_end - seek_org;
	lseek(fd_dst, seek_org, SEEK_SET);

	char buf[BUFFER_SIZE];
	int fd_src;
	ssize_t length, total = 0;
	if ((fd_src = open(pathname, O_RDONLY))<0)
	{
		fprintf(stderr, "open error %d for %s in fconcat\n", fd_src, pathname);
		exit(1);
	}
	while ((length = read(fd_src, buf, BUFFER_SIZE))>0)
		total+=length;
//	printf("DEBUG==org:%ld\tend:%ld\nresidue:%ld\n",seek_org,seek_end,residue);

	//붙여넣어질 영역의 원본 파일 밀어내기
	for(int i = 0;i<((int)residue)/BUFFER_SIZE;i++)
	{
		off_t cursor = seek_end - (i+1) * ((off_t)BUFFER_SIZE);
		lseek(fd_dst, cursor, SEEK_SET);
		length = read(fd_dst, buf, BUFFER_SIZE);
		lseek(fd_dst, cursor + (off_t)total, SEEK_SET);
		write(fd_dst, buf, length);
	}
	lseek(fd_dst, seek_org, SEEK_SET);
	length = read(fd_dst, buf, (int)residue%BUFFER_SIZE);
	lseek(fd_dst, seek_org + (off_t)total, SEEK_SET);
	write(fd_dst, buf, length);

	//src파일 붙여넣기
	lseek(fd_dst, seek_org, SEEK_SET);	
	lseek(fd_src, 0, SEEK_SET);
	while ((length = read(fd_src, buf, BUFFER_SIZE))>0)
		write(fd_dst, buf, length);
	close(fd_src);
	return total;
}

/* ---------------------------------*/
/**
 * @brief 현재 오프셋 이후의 모든 dst문자열을 src로 대체
 *
 * @param fd O_RDWR로 오픈된 파일디스크립터
 * @param dst 치환될 대상 문자열
 * @param src 삽입할 문자열
 *
 * @return 함수실행전후 파일크기 차(byte)를 리턴
 */
/* ---------------------------------*/
ssize_t freplace(int fd, const char *dst, const char *src)
{
	Off_Pair offsets;
	off_t seek_org = lseek(fd, 0, SEEK_CUR);
	off_t seek_end = lseek(fd, 0, SEEK_END);
	off_t residue;
	ssize_t length, diff, diffsum = 0;
	char buf[BUFFER_SIZE];
	lseek(fd, 0, seek_org);
	do {
		offsets = regfind(fd, dst);
		printf("so:%ld\teo:%ld\t\n",offsets.so,offsets.eo);
		if (offsets.so||offsets.eo)
		{
			lseek(fd, offsets.eo, SEEK_SET);
			residue = seek_end - offsets.eo;	//이동할 블록크기
			diff = strlen(src) - strlen(dst);	//이동할 거리
			diffsum += diff;

			if (diff != 0)
			{
				for(int i = 0;i<((int)residue)/BUFFER_SIZE;i++)
				{
					off_t cursor = seek_end - (i+1) * ((off_t)BUFFER_SIZE);
					lseek(fd, cursor, SEEK_SET);
					length = read(fd, buf, BUFFER_SIZE);
					lseek(fd, cursor + (off_t)diff, SEEK_SET);
					write(fd, buf, length);
				}
				lseek(fd, offsets.eo, SEEK_SET);
				length = read(fd, buf, (int)residue%BUFFER_SIZE);
				lseek(fd, offsets.eo + (off_t)diff, SEEK_SET);
				write(fd, buf, length);
			}

			lseek(fd, offsets.so, SEEK_SET);
			write(fd, src, strlen(src));
		}
	} while (offsets.found);

	lseek(fd, 0, SEEK_SET);
	return diffsum;
}
