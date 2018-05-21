#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <limits.h>
#include <libgen.h>
#include "ssu_backup.h"
#include "daemon.h"
#include "logger.h"
#include "util.h"
#include "error.h"

#define MAX_PROC 32768
#define FIFO_NAME "data.fifo"
/*
 *현재는 백업시 모드도 동일하게 백업하도록 구현.
 *별도의 세이브파일에 모드 저장, 정해진 모드로만 백업하도록 수정.
 *복원시 모드를 읽어와서 복원.
 */

unsigned int flag = 000;				/*option flag*/
char execname[NAME_MAX];				/*name of executable file*/
char targetpath[PATH_MAX];				/*backup file path*/
char bakdirpath[PATH_MAX] = "backup";	/*backup directory path*/
char logdirpath[PATH_MAX] = "log";		/*log directory path*/
char fifopath[PATH_MAX];
int period;
size_t bakmax; 

int main(int argc, char *argv[])
{
	/*Set options, get arguments*/
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
	strcpy(targetpath, argv[argi++]);
	if (!ON_C(flag) && !ON_R(flag))
		if ((period = atoi(argv[argi])) < 3 || period > 10)
			error(NAPRD);
	strcpy(execname, basename(argv[0]));

	/*Verify that the file exists*/
	/*Error for wrong file type*/
	struct stat filestat;
	if (stat(targetpath, &filestat) < 0)
		error(NOFILE, targetpath);
	if (!ON_D(flag) && S_ISDIR(filestat.st_mode))
		error(NEEDD, targetpath);
	if (ON_D(flag) && !S_ISDIR(filestat.st_mode))
		error(NOTDIR, targetpath);
	if (!S_ISREG(filestat.st_mode) && !S_ISDIR(filestat.st_mode))
		error(NOTREG, targetpath);

	/*Convert all pathes to absolute pathes.*/
	char abspath[PATH_MAX] = {0};
	realpath(targetpath, abspath);
	strcpy(targetpath, abspath);
	realpath(bakdirpath, abspath);
	strcpy(bakdirpath, abspath);
	realpath(logdirpath, abspath);
	strcpy(logdirpath, abspath);
	realpath(FIFO_NAME, fifopath);

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

	log_init();		/*Initialize log directory*/
	baklog(START, NULL);


	if (ON_P(flag))
	{
		remove(fifopath);
		int old = umask(0);
		mkfifo(fifopath, 0777);
		umask(old);
	}
	init_table(&table);
	kill_daemon();	/*Kill all existing backup daemon*/
	if (ON_R(flag))
		restore_bak(targetpath);
	if (ON_C(flag))
		compare_bak(targetpath);
	daemon_init();	/*Run backup daemon*/
}

static void receive_data(int fifo_fd)
{
	errlog("receiving start");

	read(fifo_fd, &table.cnt, sizeof(size_t));	
	table.be = realloc(table.be, sizeof(BakEntry)*(table.cnt));

	for (int i=0;i<(int)table.cnt;i++)
	{
		read(fifo_fd, table.be[i].filename, NAME_MAX);
		read(fifo_fd, table.be[i].abspath, PATH_MAX);
		read(fifo_fd, &table.be[i].mode, sizeof(mode_t));
		read(fifo_fd, &table.be[i].size, sizeof(size_t));
		read(fifo_fd, &table.be[i].mtime_last, sizeof(time_t));

		initQueue(&table.be[i].fileQue);
		read(fifo_fd, &table.be[i].fileQue.size, sizeof(size_t));
		errlog("Qsize:%d", table.be[i].fileQue.size);
		for (int j=0;j<(int)table.be[i].fileQue.size;j++)
		{
			errlog("test:%d",j);
			char *buf = (char*)malloc(PATH_MAX);
			read(fifo_fd, buf, PATH_MAX);
			enqueue(&(table.be[i].fileQue), buf);
			table.be[i].fileQue.size--;
		}
	}

	errlog("receiving end");
}

void kill_daemon(void)
{
	int pids[MAX_PROC], pidcnt, fifo_fd;
	/*Find all processes with process name*/
	pidcnt = findpid(execname, pids, sizeof(pids));
	/*Kill them all except itself*/
	for (int i=0;i<pidcnt;i++)
	{
		if (pids[i] == getpid())
			continue;

		printf("Send siganl to ssu_backup[%d]\n", pids[i]);

		if (ON_P(flag))
		{
			printf("Siganl[SIGUSR2]:Preserve previous entries.\n");
			kill(pids[i], SIGUSR2);

			/*Open fifo to receive backup table entries*/
			fifo_fd = open(fifopath, O_RDONLY);
			errlog("fifo opened to read");
			receive_data(fifo_fd);
			close(fifo_fd);
		}
		else
		{
			printf("Siganl[SIGUSR1]:Discard previous entries.\n");
			kill(pids[i], SIGUSR1);
		}
	}

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
	while ((c=getopt(argc, argv, "drmcn:ps")) != -1)
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
			bakmax = atoi(optarg);
			break;
		case 'p':
			flag = flag|OPT_P;
			break;
		case 's':
			flag = flag|OPT_S;
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
