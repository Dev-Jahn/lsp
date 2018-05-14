#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <libgen.h>
#include "ssu_backup.h"
#include "daemon.h"
#include "logger.h"
#include "util.h"
#include "error.h"


/*
 *현재는 백업시 모드도 동일하게 백업하도록 구현.
 *별도의 세이브파일에 모드 저장, 정해진 모드로만 백업하도록 수정.
 *복원시 모드를 읽어와서 복원.
 */

int flag = 000;							/*option flag*/
char execname[NAME_MAX];				/*name of executable file*/
char targetpath[PATH_MAX];				/*backup file path*/
char bakdirpath[PATH_MAX] = "backup";	/*backup directory path*/
char logdirpath[PATH_MAX] = "log";		/*log directory path*/
int period;

int main(int argc, char *argv[])
{
	/*Set options, parameters*/
	int argi = setopt(argc, argv);
	int argnum;
	if (ON_C(flag)||ON_R(flag))
		argnum = argi + 1;
	else
		argnum = argi + 2;
	if (argc < argnum)
		error(LESSARG);
	else if (argc > argnum)
		error(MOREARG);
	strcpy(filepath, argv[argi++]);
	if ((period = atoi(argv[argi])) < 3 || period > 10)
		error(NAPRD);
	strcpy(execname, basename(argv[0]));


	/*Verify that the file exists*/
	/*Error for wrong file type*/
	struct stat filestat;
	if (stat(filepath, &filestat) < 0)
		error(NOFILE, filepath);
	if (!ON_D(flag) && S_ISDIR(filestat.st_mode))
		error(NEEDD, filepath);
	if (ON_D(flag) && !S_ISDIR(filestat.st_mode))
		error(NOTDIR, filepath);
	if (!S_ISREG(filestat.st_mode) && !S_ISDIR(filestat.st_mode))
		error(NOTREG, filepath);

	/*Convert all pathes to absolute pathes.*/
	char abspath[PATH_MAX] = {0};
	realpath(filepath, abspath);
	strcpy(filepath, abspath);
	realpath(bakdirpath, abspath);
	strcpy(bakdirpath, abspath);
	realpath(logdirpath, abspath);
	strcpy(logdirpath, abspath);

	/*Make directory to save backup and log files.*/
	struct stat dirstat;
	if (stat(bakdirpath, &dirstat) < 0)
	{
		if (mkdir(bakdirpath, 0777) < 0)
			error(MKDIR, bakdirpath);
	}
	else if (!S_ISDIR(dirstat.st_mode))
		error(MKDIR, bakdirpath);
	if (stat(logdirpath, &dirstat) < 0)
	{
		if (mkdir(logdirpath, 0777) < 0)
			error(MKDIR, logdirpath);
	}
	else if (!S_ISDIR(dirstat.st_mode))
		error(MKDIR, logdirpath);

   /* char buf[PATH_MAX];*/
	/*strtohex(filepath, buf, sizeof(buf));*/
	/*printf("%s\n", buf);*/
	/*char buf2[PATH_MAX];*/
	/*hextostr(buf, buf2, sizeof(buf2));*/
	/*printf("%s\n", buf2);*/

	/*Initialize log directory*/
	log_init();

	/*Run backup daemon*/
	daemon_init();
}

/* ---------------------------------*/
/**
 * @brief Set option flag with entered options in command.
 *
 * @param argc argument count in main()
 * @param argv[] argument vector in main()
 *
 * @return Index of argument right after option arguments.
 */
/* ---------------------------------*/
int setopt(int argc, char *argv[])
{
	int c;
	while ((c=getopt(argc, argv, "drmcn:")) != -1)
	{
		char ch[2] = {0};
		switch(c)
		{
		case 'd':
			flag = flag|OPT_D;
			break;
		case 'r':
			flag = flag|OPT_R;
			break;
		case 'm':
			flag = flag|OPT_M;
			break;
		case 'c':
			flag = flag|OPT_C;
			break;
		case 'n':
			flag = flag|OPT_N;
			break;
		case '?':
			ch[0] = optopt;
			error(NAOPT, ch);
			break;
	}
		//If '-r' is used with other options.
		if ((flag&(~OPT_R))!=0000 && ON_R(flag))
			error(RONLY);
	}
	return optind;
}
