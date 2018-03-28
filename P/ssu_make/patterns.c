/*
 * POSIX Extended 표준 정규표현식 문법을 따르는 regex 패턴 문자열
 */

char *pat_macro		= "^\\w+\\s*(\\?)?=\\s*(\"(\\w|\\s)*\"|\\w+)\n";
char *pat_optional	= "^\\w+\\s*\\?=\\s*(\"(\\w|\\s)*\"|\\w+)";
char *pat_quot		= "^\\w+\\s*(\\?)?=\\s*\"(\\w|\\s)*\"";
char *pat_var_l 	= "\\$[\\(\\{]";
char *pat_var_r		= "[\\)\\}]";
char *pat_blank 	= "^\\s*\n";
char *pat_inner1 	= "\\$\\*";
char *pat_inner2 	= "\\$@";
char *pat_target 	= "^\\w+\\s*:(\\s*[[:graph:]]\\s*)+$";
char *pat_subtarget = "\\s*:(\\s*[[:graph:]]\\s*)+$";
char *pat_cmd 		= "[	]([[:graph:]]+\\s*)+";
