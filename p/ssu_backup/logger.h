#ifndef _LOGGER_H
#define _LOGGER_H

#include <limits.h>
#include "data.h"

extern char logpath[PATH_MAX];
enum State { 	START,		/*Program started*/
				INIT,		/*Daemon initialized*/
				BACKUP,		/*Backup file saved*/
				MODIFIED,	/*File modification detected*/
				ADDED,		/*New file created in directory*/
				DELFILE,	/*Target file has been deleted*/
				DELDIR,		/*Target dir has been deleted*/
				DELOLD,		/*Oldest backup deleted*/
				DIFF,		/*Comparing target with latest backup*/
				RESTORE,	/*Restoring target*/
				SIGNAL1,	/*Received SIGUSR1*/
				SIGNAL2,	/*Received SIGUSR2*/
				EXIT };		/*Exiting the program*/

enum ErrCode { 	NAMELIM,	/*Exceeded filename limit*/
				PATHLIM,	/*Exceeded pathname limit*/
				NAOPT,		/*Undefined option*/
				RONLY,		/*Used another option with '-r'*/
				LESSARG,	/*Not enough arguments in command*/
				MOREARG,	/*Too many arguments in command*/
				NAPRD,		/*Period value is out of range*/
				NOFILE,		/*Can't find the file with given path*/
				NEEDD,		/*Path is directory, but no '-d'*/
				NOTDIR,		/*'-d' on, but not a directory*/
				NOTREG,		/*Path is not a regular file*/
				MKDIR,		/*Error during mkdir()*/
				OPEN,		/*Error during open()*/
				STAT,		/*Error during stat()*/
				REMOVE,		/*Error during remove()*/
				SCAN,		/*Error during scandir()*/
				CHMOD,		/*Error during chmod()*/
				SAME,		/*src==dst in copy()*/
				ONFILE,		/*src:dir, dst:reg in copy()*/
				PTHCREAT,	/*Error during pthread_create()*/
				RREAL
};


void log_init(void);
void baklog(enum State st, BakEntry *bak, ...);
void filelog(const char *format, ...);
void errlog(const char *format, ...);
void error(enum ErrCode err, ...);

#endif
