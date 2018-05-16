#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include "ssu_backup.h"
#include "data.h"
#include "daemon.h"
#include "util.h"
#include "io.h"
#include "logger.h"
#include "error.h"

#define MAX_PROC 32768

void daemon_backup(const char *pathname);
void signal_handler(int signo);

BakTable table;

void daemon_main()
{
	int cnt = 0;

	while (cnt++<10)
	{
		if (ON_D(flag))
		{
		}
		else
			daemon_backup(targetpath);			
		sleep(period);
	}
	exit(0);
}

void daemon_backup(const char *abspath)
{
	/*Filename and absolute path*/
	char bakname[NAME_MAX+1] = {0};
	char bakpath[PATH_MAX] = {0};
	BakEntry *entry;
	/*Search backup entry in the table*/
	entry = search_bak(&table, abspath);
	/*If there's no entry registered*/
	if (entry == NULL)	
		entry = add_bak(&table, abspath);
	/*Delete the oldest backup file, if file count reaches limit*/
	if (ON_N(flag))
		if (entry->filecnt == bakmax)
			if(remove(entry->oldest)<0)
				error(REMOVE, entry->oldest);
	/*Make backup file name*/
	makename(abspath, bakname, sizeof(bakname));
	/*Make full path*/
	strcpy(bakpath,bakdirpath);
	strcat(bakpath, "/");
	strcat(bakpath, bakname);

	strcpy(entry->oldest, bakpath);


	copy(targetpath, bakpath);
}

int daemon_init(void)
{
	int pids[MAX_PROC];
	int pidcnt;
	/*Find all processes with process name*/
	pidcnt = findpid(execname, pids, sizeof(pids));
	/*Kill them all except itself*/
	for (int i=0;i<pidcnt;i++)
	{
		if (pids[i] == getpid())
			continue;
		printf("pid:%d\n", pids[i]);
		kill(pids[i], SIGUSR1);
	}

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

	signal(SIGUSR1, signal_handler);

	sleep(100);

	/*daemon_main();*/
	return 0;
}

void signal_handler(int signo)
{
	if (signo == SIGUSR1)
	{
		baklog(EXIT, NULL);
		exit(0);
	}
}
