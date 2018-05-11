#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <utime.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <string.h>
#include "io.h"
#include "ssu_backup.h"
#include "error.h"

#define O_OVERWR O_RDWR|O_CREAT|O_TRUNC
#define O_NOVERW O_RDWR|O_CREAT|O_EXCL
#define BUFSIZE 1024
/* ---------------------------------*/
/**
 * @brief copy src file to dst path.
 *
 * @param src Source path
 * @param dst Destination path
 */
/* ---------------------------------*/
void copy(const char *src, const char *dst)
{
	char source[PATH_MAX];
	char target[PATH_MAX];
	strcpy(source, src);
	strcpy(target, dst);
	if (strcmp(source, target) == 0)
		error(SAME,source);
	struct stat statdst;
	//dst이름의 파일이 이미 존재
	if (stat(dst, &statdst)==0)
	{
		//디렉토리이면 해당디렉토리안에 src복사
		if (S_ISDIR(statdst.st_mode))
		{
			chdir(target);
			strcpy(target, source);
			strcpy(source, "../");
			strcat(source, target);
		}
		//dst가 일반파일, src가 디렉토리면 에러
		else if (S_ISREG(statdst.st_mode)&&ON_D(flag))
			error(ONFILE,dst);
	}
	//src가 디렉토리이면
	if (ON_D(flag))
		copy_dir(source, target);
	//디렉토리가 아니면 일반복사
	else
		copy_file(source, target);
}
/* ---------------------------------*/
/**
 * @brief src가 파일일 경우에만 사용 
 *
 * @param src 복사할 원본 파일
 * @param dst 복사후 생성할 파일명
 */
/* ---------------------------------*/
void copy_file(const char *src, const char *dst)
{
	int fd1, fd2, len;
	char buf[BUFSIZE];
	if ((fd1 = open(src, O_RDONLY))<0)
		error(OPEN, src);
	if ((fd2 = open(dst, O_WRONLY|O_CREAT|O_TRUNC, 0644))<0)
		error(OPEN, dst);
	while((len = read(fd1, buf, BUFSIZE))>0)
		write(fd2, buf, len);
	/*Fix later*/
	copy_mode(src, dst);
	close(fd1);
	close(fd2);
}
/* ---------------------------------*/
/**
 * @brief scandir 엔트리에서 부모(..)와 현재디렉토리(.)를 제거
 *
 * @param dir 디렉토리의 엔트리
 *
 * @return 포함여부 
 */
/* ---------------------------------*/
int filter(const struct dirent *dir)
{
	if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
		return 0;
	else
		return 1;
}
/* ---------------------------------*/
/**
 * @brief 디렉토리에만 사용
 *
 * @param src 복사할 원본 디렉토리
 * @param dst 복사후 생성할 디렉토리 이름
 */
/* ---------------------------------*/
void copy_dir(const char *src, const char *dst)
{
	struct dirent **namelist;
	int entry;
	//디렉토리가 없으면 생성, 있으면 그냥 사용
	if (access(dst, F_OK) < 0)
	{
		if (mkdir(dst, 0777)<0)
			error(MKDIR, dst);
	}
	//src 디렉토리 내부엔트리 스캔
	if ((entry = scandir(src, &namelist, filter, alphasort))<0)
		error(SCAN, src);
	else
	{
		//엔트리마다 반복
		for (int i=0;i<entry;i++)
		{
			char srcpath[PATH_MAX];
			char dstpath[PATH_MAX];
			strcpy(srcpath, src);
			strcat(srcpath, "/"); strcat(srcpath, namelist[i]->d_name);
			strcpy(dstpath, dst);
			strcat(dstpath, "/");
			strcat(dstpath, namelist[i]->d_name);
			//디렉토리이면
			if (namelist[i]->d_type == DT_DIR)
				copy_dir(srcpath, dstpath);
			//파일이면
			else
				copy_file(srcpath, dstpath);
		}
	}
	//메모리 할당 해제
	for (int i=0;i<entry;i++)
		free(namelist[i]);
	free(namelist);
}
/* ---------------------------------*/
/**
 * @brief 권한만 복사
 *
 * @param src 복사원본
 * @param dst 대상
 */
/* ---------------------------------*/
void copy_mode(const char *src, const char *dst)
{
	struct stat statbuf;
	if (stat(src, &statbuf)<0)
		error(NOFILE, src);
	if (chmod(dst, statbuf.st_mode)<0)
		error(CHMOD, dst);
}
