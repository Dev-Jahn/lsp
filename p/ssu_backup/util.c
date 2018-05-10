#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>
#include <time.h>
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
		sprintf(buf+i*2,"%x", str[i]);
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
	return 0;
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
//hash함수 만들기
