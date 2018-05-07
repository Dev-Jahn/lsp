#ifndef _MAIN_H
#define _MAIN_H	1

#define OPT_D		0001	//디렉토리 전체백업
#define OPT_R		0002	//복구
#define OPT_M		0004	//수정시에만 백업
#define OPT_N		0010	//백업파일 유지 갯수
#define OPT_C		0020	//백업파일과 비교
#define OPT_X		0040	//예비
#define OPT_Y		0100	//예비
#define OPT_Z		0200	//예비
#define OPT_W		0400	//예비
#define ON_D(flag)	(((flag) & OPT_D) == OPT_D)
#define ON_R(flag)	(((flag) & OPT_R) == OPT_R)
#define ON_M(flag)	(((flag) & OPT_M) == OPT_M)
#define ON_N(flag)	(((flag) & OPT_N) == OPT_N)
#define ON_C(flag)	(((flag) & OPT_C) == OPT_C)
#define ON_X(flag)	(((flag) & OPT_X) == OPT_X)
#define ON_Y(flag)	(((flag) & OPT_Y) == OPT_Y)
#define ON_Z(flag)	(((flag) & OPT_Z) == OPT_Z)
#define ON_W(flag)	(((flag) & OPT_W) == OPT_W)
#define SMODE 0664

extern int flag;
int setopt(int argc, char *argv[]);
void printopt();

#endif
