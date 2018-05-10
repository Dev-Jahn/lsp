#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>
#include <time.h>
/* ---------------------------------*/
/**
 * @brief 문자열을 16진수문자열로 변환 
 *
 * @param str 변환할 대상문자열
 * @param buf 변환후 저장할 버퍼
 *
 * @return 변환후 총 길이
 */
/* ---------------------------------*/
ssize_t tohex(const char *str, char *buf, size_t bufsize)
{
	size_t len = strlen(str);
	if (len*2>=bufsize)
		return -1;
	for (size_t i=0;i<len;i++)
		sprintf(buf+i*2,"%x", str[i]);
	return len*2;
}
ssize_t makename(const char *pathname, char *buf, size_t bufsize)
{
	char timestamp[16] = {0};
	time_t now = time(NULL);
	struct tm *time_p;
	time_p = localtime(&now);
	if (tohex(pathname, buf, bufsize) < 0)
		return -1;
	else
	{
		strftime(timestamp, sizeof(timestamp), "%m%d%H%M%S", time_p);
		strcat(buf, "_");
		strcat(buf, timestamp);

		

}
//hash함수 만들기
