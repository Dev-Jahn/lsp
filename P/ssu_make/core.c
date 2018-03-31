#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "struct.h"
#include "core.h"
#include "ui.h"
#include "main.h"

/* ---------------------------------*/
/**
 * @brief 
 *
 * @param tn
 */
/* ---------------------------------*/
void execute(TNode *tn)
{
	addNode(&execed, tn);

	//실행 불필요
	if (newest(tn, 0) == 1)
		return;
	//실행 필요
	else
	{
		for (int i=0;i<(int)((Block*)tn->item)->dep_cnt;++i)
		{
			if (newest(tn->child[i], 0) == 1)
				continue;
			else
				execute(tn->child[i]);
		}

		for (int i=0;i<(int)((Block*)tn->item)->cmd_cnt;++i)
		{
			print_cmd(((Block*)tn->item)->cmds[i]);
			system(((Block*)tn->item)->cmds[i]);
		}
		//실행 불가시 예외처리
	}
}

/* ---------------------------------*/
/**
 * @brief 노드안의 타겟의 하위 디펜던시에 나중에 수정된 파일이 있는지 검사하는 함수 
 *
 * @param tn 확인할 타겟이 포함된 노드
 * @param mtime 재귀에 사용할 변수.상위노드중 존재하는 가장 가까운 노드의 최종수정시간. 초기호출시 0.
 *
 * @return 최신이면 1, 아니면 -1
 */
/* ---------------------------------*/

int newest(TNode *tn, time_t mtime)
{
	time_t comp;
	struct stat buf1, buf2;
	char *pathname = ((Block*)tn->item)->target;
	//현재노드가 없는 파일일때
	if (stat(pathname, &buf1) < 0)
	{
		if (((Block*)tn->item)->dep_cnt == 0)
			return -1;
		comp = mtime;
	}
	else
		comp = buf1.st_mtime;
	for (int i=0;i<(int)((Block*)tn->item)->dep_cnt;++i)
	{
		//dependency 파일 존재
		if (stat(((Block*)tn->item)->depends[i], &buf2) == 0)
		{	//최신이 아니면	
			if (comp < buf2.st_mtime)
				return -1;
			else
				continue;
		}
		//파일없음.하위노드 탐색
		else if (newest(tn->child[i], mtime) < 0)
			return -1;
	}
	return 1;
}
