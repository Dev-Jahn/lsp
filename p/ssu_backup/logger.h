#ifndef _LOGGER_H
#define _LOGGER_H

#include <limits.h>
#include "data.h"

/*Backup logging state*/
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

/*Error code of fatal errors*/
enum ErrCode { 	NAMELIM,	/*Exceeded filename limit*/
				PATHLIM,	/*Exceeded pathname limit*/
				NAOPT,		/*Undefined option*/
				RONLY,		/*Used another option with '-r'*/
				LESSARG,	/*Not enough arguments in command*/
				MOREARG,	/*Too many arguments in command*/
				NAPRD,		/*Period value is out of range*/
				NACNT,		/*'-n'count invalid*/
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
				PTHCREAT,	/*Error during pthread_create()*/
				RREAL		/*Used relative path in -r*/
};

void log_init(void);
void baklog(enum State st, BakEntry *bak, ...);
void errlog(const char *format, ...);
void error(enum ErrCode err, ...);

#endif
