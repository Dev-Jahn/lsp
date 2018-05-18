#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <libgen.h>
#include <pthread.h>
#include "ssu_backup.h"
#include "data.h"
#include "daemon.h"
#include "util.h"
#include "io.h"
#include "logger.h"
#include "error.h"

#define MAX_PROC 32768

void daemon_backup_dir(const char *srcdir, const char *bakdir);
void daemon_backup(const char *abspath, const char *bakdir);
void signal_handler(int signo);

BakTable table;

void daemon_main()
{
	int cnt = 0;
	init_table(&table);

	while (cnt++<3)
	{
		if (ON_D(flag))
			daemon_backup_dir(targetpath, bakdirpath);
		else
			daemon_backup(targetpath, bakdirpath);
		sleep(period);
	}
	exit(0);
}

void daemon_backup_dir(const char *srcdir, const char *bakdir)
{
	struct dirent **de;
	int dirnum;
	char tgtdir[PATH_MAX];
	strcpy(tgtdir, bakdir);
	strcat(tgtdir, "/");
	strcat(tgtdir, basename((char*)srcdir));

	errlog("srcdir:%s",srcdir);
	errlog("bakdir:%s",bakdir);


	if (access(tgtdir, F_OK) < 0)
		if (mkdir(tgtdir, 0777) < 0)
			error(MKDIR, tgtdir);

	if ((dirnum = scandir(srcdir,&de,filter_default,alphasort)) < 0)
		error(SCAN, srcdir);
	else
	{
		for (int i=0;i<dirnum;i++)
		{
			char srcpath[PATH_MAX];
			strcpy(srcpath, srcdir);
			strcat(srcpath, "/");
			strcat(srcpath, de[i]->d_name);
			if (de[i]->d_type == DT_DIR)
				daemon_backup_dir(srcpath, tgtdir);
			else
				daemon_backup(srcpath, tgtdir);
		}
	}
}

void daemon_backup(const char *abspath, const char *bakdir)
{
	/*Filename and absolute path*/
	char bakname[NAME_MAX+1] = {0};
	char *bakpath = (char*)malloc(PATH_MAX * sizeof(char));
	BakEntry *entry;
	int modified = 1;

	/*Search backup entry in the table*/
	entry = search_bak(&table, abspath);
	/*If there's no entry registered, add it to table*/
	if (entry == NULL)	
	{
		entry = add_bak(&table, abspath);
		baklog(BACKUP, entry);
	}
	else
	{
		errlog("entry:%s", entry->filename);
		errlog("files:%d",entry->fileQue.size);

		modified = check_modified(abspath, entry);
		/*Modified*/
		if (modified>0)
		{
			entry = renew_bak(&table, abspath);
			baklog(MODIFIED, entry);
		}
		/*Not modified*/
		else if (modified == 0)
		{
			if (!ON_M(flag))
				baklog(BACKUP, entry);
			else
				return;
		}
		/*File deleted*/
		else
		{
			baklog(DELETED, entry);
			if (!ON_D(flag))
				raise(SIGUSR1);
			else
			{
				remove_bak(&table, abspath);
				return;
			}
		}
	}

	if ((modified>0) || !ON_M(flag))
	{
		/*Delete the oldest backup file, if file count reaches limit*/
		if (ON_N(flag) && (entry->fileQue.size == bakmax))
		{
			char *oldest = (char*)peek(&(entry->fileQue));
			errlog("oldest:%s", oldest);
			baklog(DELOLD, entry);
			if(remove(oldest)<0)
				error(REMOVE, oldest);
			dequeue(&(entry->fileQue));
			free(oldest);
		}

		/*Make backup file name*/
		makename(abspath, bakname, sizeof(bakname));
		/*Make full path*/
		strcpy(bakpath,bakdir);
		strcat(bakpath, "/");
		strcat(bakpath, bakname);
		/*Actual file backup	*/
		copy(targetpath, bakpath);
		/*If '-n'is on, add path of backup file to the queue*/
		if (ON_N(flag))
			enqueue(&(entry->fileQue), bakpath);
	}
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
		printf("Send siganl to ssu_backup[%d]\n", pids[i]);
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

	baklog(INIT, NULL);
	daemon_main();
	return 0;
}

void signal_handler(int signo)
{
	if (signo == SIGUSR1)
	{
		baklog(EXIT, NULL);
		/*Free all heap memories used*/
		for (int i=0;i<(int)table.cnt;i++)
		{
			for (int j=0;j<(int)table.be[i].fileQue.size;j++)
				free(dequeue(&(table.be[i].fileQue)));
			free(&(table.be[i]));
		}
		exit(0);
	}
	else
		exit(1);
}
