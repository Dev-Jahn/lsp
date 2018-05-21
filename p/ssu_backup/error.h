#ifndef _ERROR_H
#define _ERROR_H

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
				REMOVE,
				SCAN,		/*Error during scandir()*/
				CHMOD,		/*Error during chmod()*/
				SAME,		/*src==dst in copy()*/
				ONFILE,		/*src:dir, dst:reg in copy()*/
				PTHCREAT	/*Error during pthread_create()*/
};
void error(enum ErrCode err, ...);

#endif
