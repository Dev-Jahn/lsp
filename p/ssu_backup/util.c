#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <openssl/sha.h>
#include "util.h"
#include "error.h"

/* ---------------------------------*/
/**
 * @brief Convert string to corresponding hexadecimal string of ASCII.
 *
 * @param str String to convert
 * @param buf Buffer to save the converted string 
 * @param bufsize size of buf
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
 * @param bufsize size of buf
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
ssize_t timestamp(char *buf, size_t  bufsize, const char *format)
{
	int len;
	time_t now = time(NULL);
	struct tm *time_p;
	time_p = localtime(&now);
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
 * @param bufsize size of buf
 *
 * @return Length of the converted string
 */
/* ---------------------------------*/
ssize_t makename(const char *pathname, char *buf, size_t bufsize)
{
	char timestamp[16] = {0};
	time_t now = time(NULL);
	struct tm *time_p;
	int len;
	time_p = localtime(&now);
	
	if ((len = strtohex(pathname, buf, bufsize)) < 0||len+11 > NAME_MAX)
		error(NAMELIM);
	else
	{
		strftime(timestamp, sizeof(timestamp), "%m%d%H%M%S", time_p);
		strcat(buf, "_");
		strcat(buf, timestamp);
	}
	return len+11;
}

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
