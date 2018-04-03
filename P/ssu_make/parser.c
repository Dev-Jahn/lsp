#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <regex.h>
#include <string.h>
#include "io.h"
#include "util.h"
#include "parser.h"
#include "patterns.h"
#include "ui.h"
#include "main.h"

TNode *root;
List entered;
/* ---------------------------------*/
/**
 * @brief 
 *
 * @param filedes
 * @param target
 * @param tn
 *
 * @return 
 */
/* ---------------------------------*/
Block *parse_Tree(int filedes, const char *target, TNode *parent)
{
	Block *blk;		//현재 타겟의 블록
	int fd = filedes, cnt;
	char line[LINE_SIZE];
	if (target == NULL)
	{	//루트블럭일때
		blk = newBlock("root");
		parent->item = blk;
		root = parent;
		Off_Pair off;
		do {
			off = regfind(fd, pat_target);
			if (off.found == 0)
				break;
			readLine(fd, line);				//한줄읽기
			char *tok = trim("^(\\w|\\.)+",line);
			lseek(fd, off.so, SEEK_SET);
			Node *n = searchList(&entered, tok, compstr);
			if (n == NULL)
				addNode(&entered, tok);
			else
			{
				close(fd);
				removetmp();
				fprintf(stderr, "ssu_make: There can't be multiple target with same name.\n");
				exit(1);
			}
			parse_Tree(fd, tok, parent);
			lseek(fd, off.eo, SEEK_SET);
		} while (off.found == 1);
	}
	else
	{
		off_t so = lseek(fd, 0, SEEK_CUR);
		readLine(fd, line);				//한줄읽기
		lseek(fd, so, SEEK_SET);
		delLine(fd);
		//타겟이름으로 블록생성
		blk = newBlock(target);
		//부모노드에 블록을 담은 자식노드 추가. 자식노드 저장.
		TNode *self = addChild(parent, blk);
		char *tok = NULL;
		char *deplist = trim(pat_dep,line);
		do
		{
			//dependency 토큰화
			if  (compare("",deplist) ==0)
				break;
			tok = trim("(\\w|\\.)+",deplist);
			deplist = sreplace(deplist,"(\\w|\\.)+\\s*","");
			if (tok == NULL)
				break;
			//이미 방문한 타겟인지 확인
			Node *c = searchList(&entered,tok, compstr);
			if (c == NULL)
				addNode(&entered, tok);
			else
			{
				TNode *circ = NULL;
				initQueue(&q);
				circ = bfstarget(root, tok);
				linkChild(self, circ);
				continue;
			}
			//블록에 dependency 추가
			addDepend(blk, tok);
			//dependency에 대한 검색패턴 구성
			char pat[WORD_SIZE] = "^";
			strcat(pat,tok);
			//.을 패턴형식으로 치환
			strcpy(pat,sreplace(pat,"\\.","\\."));
			strcat(pat, pat_subtarget);
			
			Block *childblk;
			off_t backup = lseek(fd, 0, SEEK_CUR);
			//말단이 아니면 재귀호출
			if (regfind(fd, pat).found)
				childblk = parse_Tree(fd, tok, self);
			//말단이면 파일인지 확인
			else
			{
				//파일이면 빈 블록 추가
				if (access(tok, F_OK) == 0)
				{
					childblk = newBlock(tok);
					addChild(self, childblk);
				}
				else
				{
					fprintf(stderr, "ssu_make: No rule to make target \"%s\", needed by \"%s\".\n",tok, target);
					exit(1);
				}
				//파일이 아니면 디펜던시 missing 에러
				
			}
			lseek(fd, backup, SEEK_SET);
		} while (tok != NULL);

		while ((cnt=readLine(fd, line)) != EOF)
		{
			if (compare(pat_target, line) == 0||
				compare(pat_blank, line) == 0)
				break;
			else if (compare(pat_cmd, line) == 0)
			{
				lseek(fd, -cnt-1, SEEK_CUR);
				delLine(fd);
				//커맨드 앞의 탭 제거
				tok = trim("([[:graph:]]+\\s*)+", line);
				//블록에 커맨드 추가
				addCmd(blk, tok);
				continue;
			}
			else
			{
				printf("%s\n",line);
				printf("Not a target line or command line\n");
			}
		}
	}
	return blk;	
}
