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
			{
				Node *n = searchList(&execed, tn->child[i],compPtr);
				if (n == NULL)
					execute(tn->child[i]);
				else
					printf("ssu_make: Circular dependency from \'%s\' to \'%s\'.\n",((Block*)tn->item)->target,((Block*)tn->child[i]->item)->target);
			}
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
	struct stat buf1;
	char *pathname = ((Block*)tn->item)->target;
	//초기호출시
	if (stat(pathname, &buf1) < 0)	//파일 없을때
	{
		if (mtime == 0)
			return -1;
		//현재노드가 파일이 아닌 말단 디펜던시일때
		if (((Block*)tn->item)->dep_cnt == 0)
			return -1;
		comp = mtime;
	}
	else							//파일 있을때
	{
		if (mtime == 0 )
			mtime = buf1.st_mtime;
		comp = buf1.st_mtime;
		if (mtime<comp)
			return -1;
	}
	for (int i=0;i<(int)((Block*)tn->item)->dep_cnt;++i)
	{
		int ret;
		if ((ret = newest(tn->child[i], comp)) < 0)
			return ret;
	}
	return 1;
}
