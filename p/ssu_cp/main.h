#ifndef _MAIN_H
#define _MAIN_H	1

#define OPT_S		0001	//심볼릭링크 생성
#define OPT_I		0002	//interactive
#define OPT_L		0004	//속성복사
#define OPT_N		0010	//덮어쓰지않음
#define OPT_P		0020	//속성출력
#define OPT_R		0040	//재귀복사
#define OPT_D		0100	//프로세스분할
#define OPT_B		0200	//디버깅
#define OPT_T		0400	//시간측정
#define ON_S(flag)	(((flag) & OPT_S) == OPT_S)
#define ON_I(flag)	(((flag) & OPT_I) == OPT_I)
#define ON_L(flag)	(((flag) & OPT_L) == OPT_L)
#define ON_N(flag)	(((flag) & OPT_N) == OPT_N)
#define ON_P(flag)	(((flag) & OPT_P) == OPT_P)
#define ON_R(flag)	(((flag) & OPT_R) == OPT_R)
#define ON_D(flag)	(((flag) & OPT_D) == OPT_D)
#define ON_B(flag)	(((flag) & OPT_B) == OPT_B)
#define ON_T(flag)	(((flag) & OPT_T) == OPT_T)

extern int flag;
extern int proc_num;
int setopt(int argc, char *argv[]);

#endif
