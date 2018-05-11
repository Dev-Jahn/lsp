#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "ssu_backup.h"
#include "util.h"
#include "io.h"
#include "logger.h"
#include "error.h"

#include <errno.h>
#include <openssl/sha.h>

void todo()
{
	int cnt = 0;
	/*Filename of backup file*/
	char bakname[NAME_MAX+1] = {0};
	/*Absolute path of backup file*/
	char bakpath[PATH_MAX] = {0};

	int fd_log = open(logpath, O_WRONLY|O_CREAT|O_TRUNC, 0640);
	while (cnt++<3)
	{
		makename(filepath, bakname, sizeof(bakname));
		strcpy(bakpath,bakdirpath);
		strcat(bakpath, "/");
		strcat(bakpath, bakname);
		errlog("file:%s", filepath);
		errlog("bak:%s", bakpath);
		errlog("err:%s", strerror(errno));
		copy(filepath, bakpath);
		write(fd_log, "fuck\n", 5);
		sleep(period);
	}
	exit(0);
}

int daemon_init(void)
{
	printf("Daemon initializing\n");
	pid_t pid;
	int fd, maxfd;
	/* 1)Create new child process for daemon.*/
	if ((pid = fork()) < 0)
	{
		fprintf(stderr, "fork error\n");
		exit(1);
	}
	/* Terminate parent process.*/
	else if (pid!=0)
		exit(0);
	pid = getpid();
	/* 2)Create new session.*/
	setsid();
	/* 3)Ignore job control siganls.*/
	signal(SIGTTIN, SIG_IGN);	//Ignore terminal read signal.
	signal(SIGTTOU, SIG_IGN);	//Ignore terminal write signal.
	signal(SIGTSTP, SIG_IGN);	//Ignore terminal stop siganl.
	/* 4)Close all open descriptors.*/
	maxfd = getdtablesize();
	for (fd = 0;fd < maxfd; fd++)
		close(fd);
	/* 5)Clear the file creation mask.*/
	umask(0);
	/* 6)Change working directory to root.*/
	chdir("/");
	/* 7)Redirect file descriptor 0,1,2 to null device.*/
	fd = open("/dev/null", O_RDWR);
	dup(0);
	dup(0);
	todo();
	return 0;
}


