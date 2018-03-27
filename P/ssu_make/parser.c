#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <regex.h>
#include <string.h>
#include "io.h"
#include "util.h"
#include "parser.h"
#include "patterns.h"

Command *parse_cmd(int argc, char *argv[])
{
	Command *cmd = (Command*)malloc(sizeof(Command));
	Stack tokenstk;
	tokenstk.size = 0;
	for (int i=1;i<argc;i++)
	{
		push(&tokenstk, argv[i]);
	}

	int cnt_opt=0, cnt_tar=0, cnt_mac=0;

	//옵션지정자 패턴
	char *pat_opt1 = "-\\w+";
	//인자필요 패턴
	char *pat_opt2 = "-[mhst]*[fc]";
	//인자불필요 옵션패턴 
	char *pat_opt3 = "-[mhst]+";
	//적합한 매크로패턴
	char *pat_mac = "\\w+=\\w+|\\w+=\"\\w+\"";
	char *pat_mac_l = "\\w+=";
	char *pat_mac_r = "=\\w+";
	//타겟 패턴
	char *pat_tar = "\\w+";
	//토큰 스택이 차있는 동안
	char *token;
	while (tokenstk.size != 0)
	{
		token = (char*)pop(&tokenstk);
		//매크로토큰 검출
		if (compare(pat_mac,token))
		{
			if (cnt_mac >= 5)
			{
				// 매크로갯수 초과시 예외처리
				break;
			}
			//메모리관련 수정필요
			char *k = trim(pat_mac_l, token);
			k = trim("\\w+", k);
			char *v = trim(pat_mac_r, token);
			v = trim("\\w+", v);

			cmd->macro[cnt_mac].key = k;
			cmd->macro[cnt_mac].value = v;
			cnt_mac++;
		}
		//단어형태 && 현재토큰 앞에 인자가 있을 때
		else if ((compare(pat_tar, token) == 0) && (tokenstk.top != NULL))
		{
			//앞의 인자가 인자필요패턴이면 토큰>옵션파라미터
			if (compare(pat_opt2, (char*)tokenstk.top->item) == 0)
			{
				char *prev = (char*)tokenstk.top->item;
				//끝옵션값
				for (int i = 1; i < (int)strlen(prev); i++)
				{
					char *opt = (char *)malloc(sizeof(char));
					memcpy(opt, prev + i, 1);
					cmd->opt[cnt_opt++].key = opt;
				}
				cmd->opt[cnt_opt-1].value = token;
			}
			//아니면 토큰>TARGET
			else
			{
				if (cnt_tar>=5)
				{
					//타겟개수 초과
				}
				memcpy(&(cmd->target[cnt_tar]), token, strlen(token));
				cnt_tar++;
			}
		}
	}
	return cmd;
}
void parse_body()
{
}

Block *parse_block(const char *pathname)
{
	Block *blk;
	int fd, cnt;
	char line[LINE_SIZE], *tok;
	if ((fd = open(pathname, O_RDONLY)) < 0)
	{
		fprintf(stderr, "open error for %s\n", pathname);
		exit(1);
	}
	while ((cnt = readLine(fd, line)) != EOF)
	{
		tok = strtok(line, " \t:");
		blk = newBlock(tok);
		while ((tok = strtok(NULL, " \t:")) != NULL)
			addDepend(blk, tok);

				
	}

			
		
	return blk;	
}

Tree *parse_Tree(Block *blks[], size_t amount)
{
	
}
