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

#define MAX_THREAD 100000
#define FIFO_NAME "data.fifo"

void daemon_backup_dir(const char *srcdir, const char *bakdir);
void daemon_backup(const char *abspath, const char *bakdir);
static void signal_handler(int signo);
static void send_data(int fifo_fd);

BakTable table;
pthread_t tids[MAX_THREAD];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int occupied_1 = 0;
int occupied_2 = 0;
size_t tcnt = 0;

void daemon_main()
{
	int cnt = 0;

	while (cnt++<100)
	{
		if (ON_D(flag))
		{
			daemon_backup_dir(targetpath, bakdirpath);
			for (size_t i=0;i<tcnt;i++)
				pthread_join(tids[i], NULL);
		}
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
			{
				/*struct argstr as;*/
				/*as.abspath = srcpath;*/
				/*as.bakdir = tgtdir;*/
				errlog("src:%s",srcpath);
				errlog("tgt:%s",tgtdir);

				daemon_backup(srcpath, tgtdir);
				/*if (pthread_create(&tids[tcnt], NULL, func_ptr, &as) != 0)*/
					/*error(PTHCREAT);*/
				/*tcnt++;*/
			}
		}
	}
}

void *func_ptr(void *arg)
{
	daemon_backup(((struct argstr*)arg)->abspath,
			((struct argstr*)arg)->bakdir);
	return NULL;
}

void daemon_backup(const char *abspath, const char *bakdir)
{
	/*Filename and absolute path*/
	char bakname[NAME_MAX+1] = {0};
	char *bakpath = (char*)malloc(PATH_MAX);
	BakEntry *entry;
	int modified = 1;

	/*pthread_mutex_lock(&mutex);*/
	/*while (occupied_1)*/
		/*pthread_cond_wait(&cond, &mutex);*/
	/*occupied_1 = 1;	*/

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
		/*errlog("tid:%d", pthread_self());*/
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

	/*occupied_1 = 0;*/
	/*pthread_cond_signal(&cond);*/
	/*pthread_mutex_unlock(&mutex);*/

	if ((modified>0) || !ON_M(flag))
	{
		/*pthread_mutex_lock(&mutex);*/
		/*while (occupied_2)*/
			/*pthread_cond_wait(&cond, &mutex);*/
		/*occupied_2 = 1;	*/

		/*Delete the oldest backup file, if file count reaches limit*/
		if (ON_N(flag) && (entry->fileQue.size == bakmax))
		{
			char *oldest = (char*)peek(&(entry->fileQue));
			/*errlog("oldest:%s", oldest);*/
			baklog(DELOLD, entry);
			if(remove(oldest)<0)
				error(REMOVE, oldest);
			dequeue(&(entry->fileQue));
			free(oldest);
		}

		/*occupied_2 = 0;*/
		/*pthread_cond_signal(&cond);*/
		/*pthread_mutex_unlock(&mutex);*/

		/*Make backup file name*/
		makename(abspath, bakname, sizeof(bakname));
		/*Make full path*/
		strcpy(bakpath,bakdir);
		strcat(bakpath, "/");
		strcat(bakpath, bakname);
		/*Actual file backup	*/
		copy(abspath, bakpath);
		/*If '-n'is on, add path of backup file to the queue*/
		if (ON_N(flag))
			enqueue(&(entry->fileQue), bakpath);
	}
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

	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);

	baklog(INIT, NULL);
	daemon_main();
	return 0;
}

static void signal_handler(int signo)
{
	switch (signo)
	{
		int fifo_fd;
	case SIGUSR1:
		baklog(EXIT, NULL);
		/*Free all heap memories used*/
		for (int i=0;i<(int)table.cnt;i++)
		{
			for (int j=0;j<(int)table.be[i].fileQue.size;j++)
				free(dequeue(&(table.be[i].fileQue)));
			free(&(table.be[i]));
		}
		exit(0);
	case SIGUSR2:
		/*Send backup table entries to fifo*/
		if ((fifo_fd = open(fifopath, O_WRONLY)) < 0)
			error(OPEN, fifopath);
		else
		{
			errlog("fifo opened to write");
			send_data(fifo_fd);
		}

		baklog(EXIT, NULL);
		/*Free all heap memories used*/
		for (int i=0;i<(int)table.cnt;i++)
			free(&(table.be[i]));
		close(fifo_fd);
		exit(0);
	default:
		exit(1);
	}
}

static void send_data(int fifo_fd)
{
	errlog("send data start");

	write(fifo_fd, &table.cnt, sizeof(size_t));
	for (int i=0;i<(int)table.cnt;i++)
	{
		write(fifo_fd, table.be[i].filename, NAME_MAX);
		write(fifo_fd, table.be[i].abspath, PATH_MAX);
		write(fifo_fd, &table.be[i].mode, sizeof(mode_t));
		write(fifo_fd, &table.be[i].size, sizeof(size_t));
		write(fifo_fd, &table.be[i].mtime_last, sizeof(time_t));

		write(fifo_fd, &table.be[i].fileQue.size, sizeof(size_t));
		for (int j=0;j<(int)table.be[i].fileQue.size;j++)
		{
			char *tmp = dequeue(&(table.be[i].fileQue));
			write(fifo_fd, tmp, PATH_MAX);
			free(tmp);
		}
	}

	errlog("send data end");
}
