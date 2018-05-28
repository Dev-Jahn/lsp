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
#include <fnmatch.h>
#include "ssu_backup.h"
#include "daemon.h"
#include "logger.h"
#include "util.h"

#define MAX_PROC 32768
#define FIFO_NAME "data.fifo"
#define SMODE 0664

/*
 *현재는 백업시 모드도 동일하게 백업하도록 구현.
 *별도의 세이브파일에 모드 저장, 정해진 모드로만 백업하도록 수정.
 *복원시 모드를 읽어와서 복원.
 */

unsigned int flag = 0000;				/*option flag*/
char execname[NAME_MAX];				/*name of executable file*/
char targetpath[PATH_MAX];				/*backup file path*/
char bakdirpath[PATH_MAX] = "backup";	/*backup directory path*/
char fifopath[PATH_MAX];
int period;
size_t bakmax; 

/*Internal usage*/
static void main_init(int, char *[]);
static void kill_daemon();
static void receive_data(int);
static int setopt(int, char *[]);

int main(int argc, char *argv[])
{
	log_init();				/*Initialize log directory*/
	main_init(argc, argv);	/*Initialize pathes and directories*/
	if (ON_H(flag))
	{
		usage();
		exit(0);
	}
	
	init_table(&table);		/*Initialize backup table*/
	kill_daemon();			/*Kill all existing backup daemon*/
	baklog(START, NULL);	/*Print to logfile*/

	if (ON_R(flag))			/*If -r option on*/
		restore_bak(targetpath);

	if (ON_C(flag))			/*If -c option on*/
		compare_bak(targetpath);

	daemon_init();			/*Run backup daemon*/
}

/* ---------------------------------*/
/**
 * @brief Parse arguments and set the pathes
 *
 * @param argc argc of main()
 * @param argv[] argv of main()
 */
/* ---------------------------------*/
void main_init(int argc, char *argv[])
{	/*Set options, get arguments*/
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
	if (!ON_R(flag))
	{
		if (stat(targetpath, &filestat) < 0)
			error(NOFILE, targetpath);
		if (!ON_D(flag) && S_ISDIR(filestat.st_mode))
			error(NEEDD, targetpath);
		if (ON_D(flag) && !S_ISDIR(filestat.st_mode))
			error(NOTDIR, targetpath);
		if (!S_ISREG(filestat.st_mode) && !S_ISDIR(filestat.st_mode))
			error(NOTREG, targetpath);
	}

	/*Convert all pathes to absolute pathes.*/
	char abspath[PATH_MAX] = {0};
	realpath(targetpath, abspath);
	if (ON_R(flag) && access(targetpath, F_OK) && strcmp(abspath, targetpath))
		error(RREAL);
	strcpy(targetpath, abspath);
	realpath(bakdirpath, abspath);
	strcpy(bakdirpath, abspath);
	realpath(FIFO_NAME, fifopath);

	/*Make directory to save backup.*/
	struct stat dirstat;
	if (stat(bakdirpath, &dirstat) < 0)
	{
		if (mkdir(bakdirpath, 0777) < 0)
			error(MKDIR, bakdirpath);
	}
	else if (!S_ISDIR(dirstat.st_mode))
		error(MKDIR, bakdirpath);
}

/* ---------------------------------*/
/**
 * @brief kill all processes with current process name 
 */
/* ---------------------------------*/
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
		errlog("Send siganl to ssu_backup[%d]\n", pids[i]);
		if (ON_P(flag))
		{
			umask(0);
			mkfifo(fifopath, 0777);

			errlog("(SIGUSR2) >> Preserve previous entries");
			kill(pids[i], SIGUSR2);

			/*Open fifo to receive backup table entries*/
			fifo_fd = open(fifopath, O_RDONLY);
			errlog("FIFO opened to read\n");
			receive_data(fifo_fd);
			close(fifo_fd);
			remove(fifopath);
		}
		else
		{
			errlog("(SIGUSR1) >> Discard previous entries");
			kill(pids[i], SIGUSR1);

			/*Wait until all processes are terminated*/
			while (kill(pids[i],0) == 0)
				usleep(1);
		}
	}
}

/* ---------------------------------*/
/**
 * @brief Receive backup table through named pipe
 *
 * @param fifo_fd Descriptor of named pipe
 */
/* ---------------------------------*/
void receive_data(int fifo_fd)
{
	errlog("Data receiving started");

	/*Get the table size*/
	read(fifo_fd, &table.cnt, sizeof(size_t));	
	table.be = realloc(table.be, sizeof(BakEntry)*(table.cnt));

	/*Get all entries in the table*/
	for (int i=0;i<(int)table.cnt;i++)
	{
		read(fifo_fd, table.be[i].filename, NAME_MAX);
		read(fifo_fd, table.be[i].abspath, PATH_MAX);
		read(fifo_fd, &table.be[i].mode, sizeof(mode_t));
#ifdef HASH
		read(fifo_fd, &table.be[i].checksum_last, 65);
#else
		read(fifo_fd, &table.be[i].mtime_last, sizeof(time_t));
#endif

		initQueue(&table.be[i].fileQue);
		read(fifo_fd, &table.be[i].fileQue.size, sizeof(size_t));
		errlog("Entry:%s  Queue:%d", table.be[i].filename, table.be[i].fileQue.size);
		/*Transfer file queue to entry*/
		for (int j=0;j<(int)table.be[i].fileQue.size;j++)
		{
			char *buf = (char*)malloc(PATH_MAX);
			read(fifo_fd, buf, PATH_MAX);
			enqueue(&(table.be[i].fileQue), buf);
			table.be[i].fileQue.size--;
		}
	}

	errlog("Data receiving completed");
	printf("\n");
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
#ifdef HASH
	while ((c=getopt(argc, argv, "drmcpshn:")) != -1)
#else
	while ((c=getopt(argc, argv, "drmcphn:")) != -1)
#endif
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
			if (fnmatch("[0-9]*", optarg, 0))
				error(NACNT);
			bakmax = atoi(optarg);
			break;
		case 'p':
			flag = flag|OPT_P;
			break;
#ifdef HASH
		case 's':
			flag = flag|OPT_S;
			break;
#endif
		case 'h':
			flag = flag|OPT_H;
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

/* ---------------------------------*/
/**
 * @brief Print usage
 */
/* ---------------------------------*/
void usage(void)
{
	printf("\n[Usage]\n");
	printf("ssu_backup <FILENAME> [PERIOD]... [OPTION]...\n");
	printf("[PERIOD] Integer from 3 to 10\n");
	printf("[OPTION] -d\tBackup directory\n");
	printf("         -m\tBackup only if modified\n");
	printf("         -n N\tMaintain N backups\n");
	printf("         -r\tRestore from backups\n");
	printf("         -c\tCompare with backup\n");
	printf("         -p\tPreserve previous table\n");
#ifdef HASH 
	printf("         -s\tStrict modification check with SHA256\n");
#endif
}
