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
#include "main.h"
#include "copy.h"
#include "error.h"

#define O_OVERWR O_RDWR|O_CREAT|O_TRUNC
#define O_NOVERW O_RDWR|O_CREAT|O_EXCL
#define BUFSIZE 1024
/* ---------------------------------*/
/**
 * @brief 원본의 타입과 옵션에 따라 복사를 수행 
 *
 * @param src 복사할 원본 파일 또는 디렉토리
 * @param tgt 복사후 생성할 파일 또는 디렉토리
 */
/* ---------------------------------*/
void copy(const char *src, const char *tgt)
{
	char source[PATH_MAX];
	char target[PATH_MAX];
	strcpy(source, src);
	strcpy(target, tgt);
	//상대, 절대경로 혼용할때도 예외처리 가능하도록 수정할것
	if (strcmp(source, target) == 0)
		error(SAME,source);
	struct stat statsrc, stattgt;
	if (stat(source, &statsrc)<0)
		error(NOFILE, source);
	//tgt이름의 파일이 이미 존재
	if (stat(tgt, &stattgt)==0)
	{
		//디렉토리이면 해당디렉토리안에 src복사
		if (S_ISDIR(stattgt.st_mode))
		{
			chdir(target);
			strcpy(target, source);
			strcpy(source, "../");
			strcat(source, target);
		}
		//tgt가 일반파일, src가 디렉토리면 에러
		else if (S_ISREG(stattgt.st_mode)&&S_ISDIR(statsrc.st_mode))
			error(ONFILE,tgt);
	}
	//src가 디렉토리이면
	if (S_ISDIR(statsrc.st_mode))
	{
		//r옵션이 있으면 재귀복사
		if (ON_R(flag))
			copy_dir(source, target);
		//d옵션이 있으면 프로세스 분할복사
		else if (ON_D(flag))
			copy_proc(source, target);
		//둘다 없으면 에러
		else
			error(ISDIR, source);
	}
	//디렉토리가 아니면 일반복사
	else
	{
		if (ON_R(flag)||ON_D(flag))
			error(NOTDIR, source);
		else
			copy_file(source, target);
	}
	//l옵션이 있으면 속성복사
	if (ON_L(flag))
		copy_stat(source, target);
}
/* ---------------------------------*/
/**
 * @brief src가 파일일 경우에만 사용 
 *
 * @param src 복사할 원본 파일
 * @param tgt 복사후 생성할 파일명
 */
/* ---------------------------------*/
void copy_file(const char *src, const char *tgt)
{
	int fd1, fd2, len;
	char buf[BUFSIZE];
	char answer[BUFSIZE];
	if ((fd1 = open(src, O_RDONLY))<0)
		error(OPEN, src);
	if (ON_N(flag))
	{
		if ((fd2 = open(tgt, O_NOVERW, SMODE))<0)
		{
			if (ON_B(flag))
				printf("'%s' already exist.", tgt);
			return;
		}
	}
	else if (ON_I(flag))
	{
		printf("Do you want to overwrite '%s'?(y/n) ", tgt);
		scanf("%s",answer);
		if (strcmp(answer, "y") != 0)
		{
			if (ON_B(flag))
				printf("No overwrite\n");
			return;
		}
		else
			if (ON_B(flag))
				printf("Overwrite\n");
	}
	if ((fd2 = open(tgt, O_OVERWR, SMODE))<0)
		error(OPEN, tgt);
	while((len = read(fd1, buf, BUFSIZE))>0)
		write(fd2, buf, len);
	if (ON_L(flag))
		copy_stat(src, tgt);
	/*else*/
		/*copy_mode(src, tgt);*/
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
 * @param tgt 복사후 생성할 디렉토리 이름
 */
/* ---------------------------------*/
void copy_dir(const char *src, const char *tgt)
{
	struct dirent **namelist;
	int entry;
	//디렉토리가 없으면 생성, 있으면 그냥 사용
	if (access(tgt, F_OK) < 0)
	{
		if (mkdir(tgt, 0777)<0)
			error(MKDIR, tgt);
	}
	//l옵션이 있으면 속성복사
	if (ON_L(flag))
		copy_stat(src, tgt);
	//없으면 권한만 복사
	else
		copy_mode(src, tgt);
	//src 디렉토리 내부엔트리 스캔
	if ((entry = scandir(src, &namelist, filter, alphasort))<0)
		error(SCAN, src);
	else
	{
		//엔트리마다 반복
		for (int i=0;i<entry;i++)
		{
			char srcpath[PATH_MAX];
			char tgtpath[PATH_MAX];
			strcpy(srcpath, src);
			strcat(srcpath, "/");
			strcat(srcpath, namelist[i]->d_name);
			strcpy(tgtpath, tgt);
			strcat(tgtpath, "/");
			strcat(tgtpath, namelist[i]->d_name);
			//디렉토리이면
			if (namelist[i]->d_type == DT_DIR)
				copy_dir(srcpath, tgtpath);
			//파일이면
			else
				copy_file(srcpath, tgtpath);
		}
	}
	//메모리 할당 해제
	for (int i=0;i<entry;i++)
		free(namelist[i]);
	free(namelist);
}
/* ---------------------------------*/
/**
 * @brief 프로세스분할복사
 *
 * @param src 원본 디렉토리
 * @param tgt 생성할 디렉토리
 */
/* ---------------------------------*/
void copy_proc(const char *src, const char *tgt)
{
	int proc_cnt = 0;
	struct dirent **namelist;
	int entry;
	struct stat statbuf;
	if (stat(src, &statbuf)<0)
		error(NOFILE, src);
	if (mkdir(tgt, statbuf.st_mode)<0)
		error(MKDIR, tgt);
	printf("pid:%d(parent)\tdir:%s\n",getpid(),tgt);
	if ((entry = scandir(src, &namelist, filter, alphasort))<0)
		error(SCAN, src);
	else
	{
		int pid, wpid;
		int status = 0;
		for (int i=0;i<entry;i++)
		{
			char srcpath[PATH_MAX];
			char tgtpath[PATH_MAX];
			strcpy(srcpath, src);
			strcat(srcpath, "/");
			strcat(srcpath, namelist[i]->d_name);
			strcpy(tgtpath, tgt);
			strcat(tgtpath, "/");
			strcat(tgtpath, namelist[i]->d_name);
			if (namelist[i]->d_type == DT_DIR)
			{
				if (proc_cnt < proc_num)
				{
					if ((pid = fork()) == 0)
					{
						copy_dir(srcpath, tgtpath);
						printf("pid:%d\t\tdir:%s\n",getpid(),tgtpath);
						exit(0);
					}
						proc_cnt++;
				}
				else
					copy_dir(srcpath, tgtpath);
			}
			else
				copy_file(srcpath, tgtpath);
		}
		//모든 자식프로세스의 종료를 기다림
		while ((wpid = wait(&status)) > 0);
		
	}
}
/* ---------------------------------*/
/**
 * @brief 속성복사(권한, 소유자, 액세스, 수정시간)
 *
 * @param src 복사원본
 * @param tgt 대상
 */
/* ---------------------------------*/
void copy_stat(const char *src, const char *tgt)
{
	struct stat statbuf;
	struct utimbuf timebuf;
	if (stat(src, &statbuf)<0)
		error(NOFILE, src);
	if (chmod(tgt, statbuf.st_mode)<0)
		error(CHMOD, tgt);
	if (chown(tgt, statbuf.st_uid, statbuf.st_gid)<0)
		error(CHOWN, tgt);
	timebuf.actime = statbuf.st_atime;
	timebuf.modtime = statbuf.st_mtime;
	if (utime(tgt,&timebuf)<0)
		error(UTIME, tgt);
}
/* ---------------------------------*/
/**
 * @brief 권한만 복사
 *
 * @param src 복사원본
 * @param tgt 대상
 */
/* ---------------------------------*/
void copy_mode(const char *src, const char *tgt)
{
	struct stat statbuf;
	if (stat(src, &statbuf)<0)
		error(NOFILE, src);
	if (chmod(tgt, statbuf.st_mode)<0)
		error(CHMOD, tgt);
}
/* ---------------------------------*/
/**
 * @brief 속성출력
 *
 * @param src 출력할 파일
 */
/* ---------------------------------*/
void print_stat(const char *src)
{
	struct stat statbuf;
	struct tm *atime_p, *mtime_p, *ctime_p;
	struct passwd *usr;
	struct group *grp;
	if (stat(src, &statbuf)<0)
		error(NOFILE, src);
	atime_p = localtime(&statbuf.st_atime);
	mtime_p = localtime(&statbuf.st_mtime);
	ctime_p = localtime(&statbuf.st_ctime);
	usr = getpwuid(statbuf.st_uid);
	grp = getgrgid(statbuf.st_gid);

	printf("************** File Info **************\n");
	printf("Filename           : %s\n",src);
	printf("Last access time   : %d.%d.%d %d:%d:%d\n",
			atime_p->tm_year+1900, atime_p->tm_mon+1, atime_p->tm_mday, atime_p->tm_hour, atime_p->tm_min, atime_p->tm_sec);
	printf("Last modified time : %d.%d.%d %d:%d:%d\n",
			mtime_p->tm_year+1900, mtime_p->tm_mon+1, mtime_p->tm_mday, mtime_p->tm_hour, mtime_p->tm_min, mtime_p->tm_sec);
	printf("Last changed time  : %d.%d.%d %d:%d:%d\n",
			ctime_p->tm_year+1900, ctime_p->tm_mon+1, ctime_p->tm_mday, ctime_p->tm_hour, ctime_p->tm_min, ctime_p->tm_sec);
	printf("Owner              : %s\n", usr->pw_name);
	printf("Group              : %s\n", grp->gr_name);
	printf("File size          : %ld Bytes\n", statbuf.st_size);

}
