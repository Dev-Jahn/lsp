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
#include "gc.h"

/*#define MAX_HEAP 4294967296*/
#define MAX_HEAP 41943040
#define MAX_THREAD 100000
#define FIFO_NAME "data.fifo"

void daemon_backup_dir(const char *srcdir, const char *bakdir);
void daemon_backup(const char *abspath, const char *bakdir);
static void setsig();
static void signal_handler(int signo, siginfo_t *info, void *);
static void send_data(int fifo_fd);
static void lock();
static void unlock();

BakTable table;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int occupied = 0, first = 1;
Queue threadQue;

void daemon_main()
{
	sigset_t set, oldset;
	sigfillset(&set);
	sigdelset(&set,SIGABRT);

	if (!ON_P(flag))
		load_table(&table, targetpath);
	initQueue(&threadQue);
	gc_start(MAX_HEAP, 0.5);

	while (1)
	{
		gc_check();
		/*Block all signals during backup*/
		sigprocmask(SIG_BLOCK, &set, &oldset);

		if (ON_D(flag))
		{
			daemon_backup_dir(targetpath, bakdirpath);
			first = 0;
			while(threadQue.size)
			{
				pthread_t *tid = (pthread_t*)dequeue(&threadQue);
				/*errlog("joining %lu",*tid);*/
				pthread_join(*tid, NULL);
				free(tid);
			}

			/*Check the file which is not updated*/
			/*Remove entry if not updated*/
			for (int i=0;i<(int)table.cnt;i++)
				if(!table.be[i].updateflag)
				{
					baklog(DELFILE, &table.be[i]);
					remove_entry(&table,table.be[i].abspath);
					i--;
				}
			/*Set update flag to 0*/
			for (int i=0;i<(int)table.cnt;i++)
				table.be[i].updateflag = 0;
		}
		else
			daemon_backup(targetpath, bakdirpath);

		/*Recover to default mask*/
		sigprocmask(SIG_SETMASK, &oldset, NULL);

		sleep(period);
	}
	exit(0);
}

void daemon_backup_dir(const char *srcdir, const char *bakdir)
{
	struct dirent **de;
	int dirnum;

	if (access(bakdir, F_OK) < 0)
		if (mkdir(bakdir, 0777) < 0)
			error(MKDIR, bakdir);

	/*When target directory is deleted, exit*/
	if ((dirnum = scandir(srcdir,&de,filter_default,alphasort)) < 0)
	{
		baklog(DELDIR, NULL, basename((char*)srcdir));
		raise(SIGABRT);
	}

	for (int i=0;i<dirnum;i++)
	{
		if (de[i]->d_ino == 0)
			continue;
		struct argstr *as = (struct argstr*)malloc(sizeof(struct argstr));
		gc_enqueue(as, sizeof(struct argstr));
		strcpy(as->bakdir, bakdirpath);
		strcpy(as->abspath, srcdir);
		strcat(as->abspath, "/");
		strcat(as->abspath, de[i]->d_name);
		/*If directory, recursive call*/
		if (de[i]->d_type == DT_DIR)
			daemon_backup_dir(as->abspath, as->bakdir);
		/*If file, create new thread and do backup*/
		else
		{
			BakEntry *e = search_entry(&table, as->abspath);
			if (e!=NULL)
				e->updateflag = 1;
			else if (!first)
			{
				pthread_mutex_lock(&mutex);
				baklog(ADDED, NULL, de[i]->d_name, srcdir);
				pthread_mutex_unlock(&mutex);
			}

			pthread_t *tid = (pthread_t*)malloc(sizeof(pthread_t));
			enqueue(&threadQue, tid);
			if (pthread_create(tid, NULL, func_ptr, as) != 0)
				error(PTHCREAT);
		}
	}
	for (int i=0;i<dirnum;i++)
		free(de[i]);
	free(de);
}

void *func_ptr(void *arg)
{
	daemon_backup(((struct argstr*)arg)->abspath,
			((struct argstr*)arg)->bakdir);
	/*errlog("Thread END//tid:%lu/path:%s",pthread_self(),((struct argstr*)arg)->abspath);*/

	return arg;
}
void daemon_backup(const char *abspath, const char *bakdir)
{
	BakEntry *entry;
	int modified = 0;
	/*Filename and absolute path*/
	char bakname[NAME_MAX+1] = {0};
	char *bakpath = (char*)malloc(PATH_MAX);
	/*Make backup file name*/
	makename(abspath, bakname, sizeof(bakname));
	/*Make full path*/
	strcpy(bakpath,bakdir);
	strcat(bakpath, "/");
	strcat(bakpath, bakname);
	
	/*Lock the critical section*/
	lock();	

	/*Search backup entry in the table*/
	entry = search_entry(&table, abspath);

	/*If there's no entry registered, add it to table*/
	if (entry == NULL)	
	{
		errlog("Added %s to table", abspath);
		entry = load_entry(&table, abspath);
		modified = 1;
	}

	/*Check if file is modified*/
	modified = modified ? 1 : check_modified(abspath, entry);

	/*When file deleted*/
	if (modified<0)
	{
		baklog(DELFILE, entry);
		raise(SIGABRT);
	}

	/*Delete the oldest backup file, if file count reaches limit*/
	size_t baklim = (ON_M(flag) && modified==0) ? bakmax+1 : bakmax;
	if (ON_N(flag) && (entry->fileQue.size >= baklim))
	{
		char *oldest;
		for (int i=0;i<(int)(entry->fileQue.size-baklim+1);)
		{
			/*Just peek cuz there's use of oldest in logging*/
			oldest = (char*)peek(&(entry->fileQue));
			baklog(DELOLD, entry);
			if(remove(oldest)<0)
				error(REMOVE, oldest);
			free(dequeue(&(entry->fileQue)));
		}
	}

	/*Logging*/
	if (modified>0)
	{
		entry = renew_entry(&table, abspath);
		baklog(MODIFIED, entry);
	}
	else if (!ON_M(flag))
		baklog(BACKUP, entry);
	else
	{
		unlock();
		return;
	}

	/*Unlock the critical section*/
	unlock();

	
	/*Actual file backup	*/
	copy(abspath, bakpath);
	/*If '-n'is on, add path of backup file to the queue*/
	if (ON_N(flag))
		enqueue(&(entry->fileQue), bakpath);
	else
		free(bakpath);
}

int daemon_init(void)
{
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
	{
		errlog("Daemon initializing[%d]",pid);
		exit(0);
	}
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

	setsig();

	baklog(INIT, NULL);
	daemon_main();
	return 0;
}

static void setsig()
{
	struct sigaction act;
	sigfillset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = signal_handler;

	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	sigaction(SIGABRT, &act, NULL);
}

static void signal_handler(int signo, siginfo_t *info, void *arg)
{
	arg++;
	switch (signo)
	{
		int fifo_fd;
	case SIGABRT:
		baklog(EXIT, NULL);
		break;

	case SIGUSR1:
		baklog(SIGNAL1, NULL, info->si_pid);
		baklog(EXIT, NULL);
		break;

	case SIGUSR2:
		baklog(SIGNAL2, NULL, info->si_pid);
		baklog(EXIT, NULL);
		/*Send backup table entries to fifo*/
		if ((fifo_fd = open(fifopath, O_WRONLY)) < 0)
			error(OPEN, fifopath);
		else
			send_data(fifo_fd);
		close(fifo_fd);
		break;

	default:
		exit(1);
	}
	exit(0);
}

static void send_data(int fifo_fd)
{
	errlog("Data sending started");

	write(fifo_fd, &table.cnt, sizeof(size_t));
	for (int i=0;i<(int)table.cnt;i++)
	{
		errlog("name:%s",table.be[i].filename);
		write(fifo_fd, table.be[i].filename, NAME_MAX);
		write(fifo_fd, table.be[i].abspath, PATH_MAX);
		write(fifo_fd, &table.be[i].mode, sizeof(mode_t));
		write(fifo_fd, &table.be[i].mtime_last, sizeof(time_t));

		errlog("Qsize:%ld",table.be[i].fileQue.size);
		write(fifo_fd, &table.be[i].fileQue.size, sizeof(size_t));
		for (int j=0;j<(int)table.be[i].fileQue.size;)
		{
			char *tmp = dequeue(&(table.be[i].fileQue));
			write(fifo_fd, tmp, PATH_MAX);
		}
	}

	errlog("Data sending completed");
}

static void lock()
{
	if (ON_D(flag))
	{
		pthread_mutex_lock(&mutex);
		while (occupied)
			pthread_cond_wait(&cond, &mutex);
		occupied = 1;	
	}
}

static void unlock()
{
	if (ON_D(flag))
	{
		occupied = 0;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
}
