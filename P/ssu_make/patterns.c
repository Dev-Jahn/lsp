#include "patterns.h"
/*
 * POSIX Extended 표준 정규표현식 문법을 따르는 regex 패턴 문자열
 */

const char *pat_macro		= "^\\w+\\s*(\\?)?=\\s*([^ 	]+[ 	]*)+$";
const char *pat_essential	= "^\\w+\\s*=\\s*([^ 	]+\\s*)+$";
const char *pat_optional	= "^\\w+\\s*(\\?)=\\s*([^ 	]+\\s*)+$";
const char *pat_var_l 		= "\\$[\\(\\{]";
const char *pat_var_r		= "[\\)\\}]";
const char *pat_blank 		= "^[ 	]*$";
const char *pat_blank_lf	= "^\\s*\n";
const char *pat_comment		= "^#.*";
const char *pat_inner 		= "\\$@";
const char *pat_inner_noex	= "\\$\\*";
const char *pat_inner_dep	= "\\$\\^";
/*const char *pat_target 		= "^(\\w|\\.)+\\s*:\\s*((\\w|\\.)+[ 	]*)*$";*/
const char *pat_target 		= "^[^ 	]+\\s*:\\s*([^ 	]+[ 	]*)*$";
const char *pat_dep			= "((\\w|\\.)+\\s*)*$";
const char *pat_subtarget	= "\\s*:(\\s*[^ 	]+\\s*)*$";
const char *pat_cmd 		= "[	]([^ 	]+\\s*)+";
const char *pat_terminal	= "\\w+\\.(c|h)";
const char *pat_ext			= "\\.\\w";
const char *pat_include		= "^include[ 	]*((\\w|\\.)+[ 	]*)*$";
const char *pat_newline		= "\\\\\n";
