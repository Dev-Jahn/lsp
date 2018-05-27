#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <fnmatch.h>
#include <limits.h>
#include <time.h>
#ifdef SHA
#include <openssl/sha.h>
#endif
#include "util.h"
#include "logger.h"

/* ---------------------------------*/
/**
 * @brief Convert string to corresponding hexadecimal string of ASCII.
 *
 * @param str String to convert
 * @param buf Buffer to save the converted string 
 * @param bufsize Size of buffer
 *
 * @return Length of the converted string
 */
/* ---------------------------------*/
ssize_t strtohex(const char *str, char *buf, size_t bufsize)
{
	size_t len = strlen(str);
	if (len*2>=bufsize)
		return -1;
	for (size_t i=0;i<len;i++)
		sprintf(buf+i*2,"%02x", str[i]);
	return len*2;
}

/* ---------------------------------*/
/**
 * @brief Convert hexadeciaml string to corresponding ASCII string.
 *
 * @param str String to convert
 * @param buf Buff
 * @param buf Buffer to save the converted string 
 * @param bufsize Size of buffer
 *
 * @return Length of the converted string
 */
/* ---------------------------------*/
ssize_t hextostr(const char *str, char *buf, size_t bufsize)
{
	size_t len = strlen(str);
	char tmp[5];
	if (len/2>=bufsize)
		return -1;
	for (size_t i = 0;i<len/2;i++)
	{
		strcpy(tmp,"0x");
		strncat(tmp,str+i*2,2);
		sprintf(buf+i, "%c", (int)strtol(tmp,NULL,0));
	}
	return len/2;
}

/* ---------------------------------*/
/**
 * @brief Make timestamp with given format
 *
 * @param when Time to create stamp
 * @param buf Buffer to save timestamp
 * @param bufsize Size of buffer
 * @param format Format string
 *
 * @return Length of timestamp
 */
/* ---------------------------------*/
ssize_t timestamp(time_t when, char *buf, size_t  bufsize, const char *format)
{
	int len;
	struct tm *time_p;
	time_p = localtime(&when);
	if ((len = strftime(buf, bufsize, format, time_p)) == 0)
		return -1;
	else
		return len;
}

/* ---------------------------------*/
/**
 * @brief Convert the path string to 'hexadecimal-code_time-stamp' form.
 *
 * @param pathname Absolute path of target file
 * @param buf Buffer to save the converted string
 * @param bufsize Size of buffer
 *
 * @return Backup time
 */
/* ---------------------------------*/
time_t makename(const char *pathname, char *buf, size_t bufsize)
{
	char stamp[16] = {0};
	int len;
	time_t btime = time(NULL);
	
	if ((len = strtohex(pathname, buf, bufsize)) < 0||len+11 > NAME_MAX)
		error(NAMELIM);
	else
	{
		timestamp(btime, stamp, sizeof(stamp), "%m%d%H%M%S");
		strcat(buf, "_");
		strcat(buf, stamp);
	}
	return btime;
}

/* ---------------------------------*/
/**
 * @brief Get pointer of timestamp part from backup filename.
 *
 * @param bakname Filename excluding path
 *
 * @return Position of timestamp starting
 */
/* ---------------------------------*/
char *getbtime(const char *bakname)
{
	size_t len = strlen(bakname);
	for (int i=len-1;i>=0;i--)
		if (bakname[i] == '_')
			return (char*)(bakname+i+1);
	return NULL;
}

/* ---------------------------------*/
/**
 * @brief Get hexadecimal part from backup filename.
 *
 * @param bakname Filename excluding path
 *
 * @return Heap allocated string(Need to be freed)
 */
/* ---------------------------------*/
char *gethexname(const char *bakname)
{
	char *name = (char*)malloc(NAME_MAX);
	size_t len = strlen(bakname);
	for (int i=0;i<(int)len;i++)
		if (bakname[i] == '_')
		{
			strncpy(name, bakname, i);
			name[i] = 0;
			if (fnmatch("[0-9a-f]*",name,0))
				return NULL;
			else
				return name;
		}
	return NULL;
}

#ifdef SHA
/* ---------------------------------*/
/**
 * @brief Hash the file to 32 byte, convert to 64 digit hex string
 *
 * @param pathname File to hash
 * @param output Buffer to save result
 *
 * @return Size of the file, -1 when error
 */
/* ---------------------------------*/
int sha256_file(const char *pathname, char output[SHA256_DIGEST_LENGTH*2+1])
{
	int fd, nbytes = 0, filesize = 0;
    const int bufsize = 4096;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    unsigned char buf[4096];
    SHA256_CTX sha256;

	if ((fd = open(pathname, O_RDONLY)) < 0)
		return -1;
	/*Initialize*/
    SHA256_Init(&sha256);
	/*Hash each of chunks*/
    while((nbytes = read(fd, buf, bufsize)) > 0)
	{
        SHA256_Update(&sha256, buf, nbytes);
		filesize+=nbytes;
	}
	/*Get the 32 byte hash value*/
    SHA256_Final(hash, &sha256);
	close(fd);

	/*Make 64 letters hexadecimal string*/
	for (int i = 0;i<SHA256_DIGEST_LENGTH;i++)
		sprintf(output+i*2, "%02x", hash[i]);
	output[64] = 0;

    return filesize;
}
#endif

/* ---------------------------------*/
/**
 * @brief Remove '..' and '.' from entries
 *
 * @param dir Directory entry
 *
 * @return To include, 1. To exclude, 0.
 */
/* ---------------------------------*/
int filter_default(const struct dirent *dir)
{
	if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
		return 0;
	else
		return 1;
}

/* ---------------------------------*/
/**
 * @brief Get pid form only
 *
 * @param dir Directory entry
 *
 * @return If pid, 1. Else 0
 */
/* ---------------------------------*/
int filter_pid(const struct dirent *dir)
{
	return !fnmatch("[0-9]*", dir->d_name, 0);
}

/* ---------------------------------*/
/**
 * @brief Find the all pids with matching name 
 *
 * @param procname Name of the process to find
 * @param procs Buffer array to save pid
 * @param size Size of buffer
 *
 * @return Number of pids found
 */
/* ---------------------------------*/
int findpid(const char *procname, int *pidbuf, size_t maxpid)
{
	struct dirent **de;
	char pathname[NAME_MAX];
	char execname[NAME_MAX];
	char buf[1024], *ptr;
	int dircnt, pidcnt=0, fd;
	pid_t pid;
	if ((dircnt = scandir("/proc", &de, filter_pid, alphasort))<0)
		return -1;
	else if (dircnt > (int)maxpid)
	{
		for (int i=0;i<dircnt;i++)
			free(de[i]);
		free(de);
		return -1;
	}
	else
	{
		for (int i=0;i<dircnt;i++)
		{
			if (de[i]->d_ino == 0)
				continue;
			strcpy(pathname, "/proc/");
			strcat(pathname, de[i]->d_name);
			strcat(pathname, "/stat");
			if ((fd = open(pathname, O_RDONLY))<0)
				continue;
			else
			{
				read(fd, buf, 1024);
				ptr = strtok(buf, " ()");
				pid = atoi(ptr);
				ptr = strtok(NULL, " ()");
				strcpy(execname, ptr);
				if (strcmp(execname, procname) == 0)
				{
					pidbuf[pidcnt] = pid;
					pidcnt++;
				}
			}
		}
		for (int i=0;i<dircnt;i++)
			free(de[i]);
		free(de);

		return pidcnt;
	}
}

/* ---------------------------------*/
/**
 * @brief Print the contents of the file
 *
 * @param abspath Absolute path of target file
 *
 * @return Success, 0. Else -1.
 */
/* ---------------------------------*/
ssize_t cat(const char *abspath)
{
	FILE *fp;
	char buf[1024];
	if ((fp = fopen(abspath, "r")) == NULL)
		return -1;
	while (fgets(buf, sizeof(buf), fp) != NULL)
		fputs(buf,stdout);
	return 0;
}
