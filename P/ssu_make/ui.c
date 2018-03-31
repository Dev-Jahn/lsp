#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "main.h"
#include "ui.h"
#include "util.h"

struct winsize window;
char lrd[] = { 0xe2, 0x95, 0xa6, '\0' };//╦
char ud[]  = { 0xe2, 0x95, 0x91, '\0' };//║
char rud[] = { 0xe2, 0x95, 0xa0, '\0' };//╠
char ru[]  = { 0xe2, 0x95, 0x9a, '\0' };//╚
char lr[]  = { 0xe2, 0x95, 0x90, '\0' };//═
int width = 0;
List indent;
List printed;
void set_ui_env()
{
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
}

void print_cmd(const char *cmd)	//s플래그 선언여부에 따라 커맨드 출력
{
	if (!ON_S(flag))
		printf("%s\n",cmd);
}

void print_help()				//h플래그 선언시 사용법출력
{
	set_ui_env();
	if (ON_H(flag))
	{
		for (int i=0;i<window.ws_col;i++)
			printf("*");
		printf("\n");
		printf("%s","\
\t███████╗███████╗██╗   ██╗    ███╗   ███╗ █████╗ ██╗  ██╗███████╗\n\
\t██╔════╝██╔════╝██║   ██║    ████╗ ████║██╔══██╗██║ ██╔╝██╔════╝\n\
\t███████╗███████╗██║   ██║    ██╔████╔██║███████║█████╔╝ █████╗  \n\
\t╚════██║╚════██║██║   ██║    ██║╚██╔╝██║██╔══██║██╔═██╗ ██╔══╝  \n\
\t███████║███████║╚██████╔╝    ██║ ╚═╝ ██║██║  ██║██║  ██╗███████╗\n\
\t╚══════╝╚══════╝ ╚═════╝     ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝\n");
		for (int i=0;i<window.ws_col;i++)
			printf("*");
		printf("\n");
		printf("  Usage : ssu_make [Target] [Option] [Macro]\n\n");
		printf("  Option:\n\
   -f <file>	Use <file> as a makefile.\n\
   -c <directory>	Change to directory <directory> before reading the makefiles.\n\
   -s		Do not print the commands as they are executed.\n\
   -h		Print usage.\n\
   -m		Print macro list.\n\
   -t		Print dependency graph as a tree.\n");
		for (int i=0;i<window.ws_col;i++)
			printf("*");
		printf("\n");
	}
}

void print_mac(List *ml)		//m플래그 선언여부에 따라 매크로 출력
{
	set_ui_env();
	char str[20] = " Defined Macros ";
	for (int i=0;i<(window.ws_col-(int)strlen(str))/2;i++)
		printf("*");
	printf("%s",str);
	for (int i=0;i<(window.ws_col-(int)strlen(str))/2;i++)
		printf("*");
	printf("\n");
	for (ml->cur=ml->head;ml->cur!=NULL;ml->cur=ml->cur->next)
	{
		printf("%s=",((Pair*)ml->cur->item)->key);
		printf("%s\n",((Pair*)ml->cur->item)->value);
	}
	for (int i=0;i<window.ws_col;i++)
		printf("*");
	printf("\n");
}

/* ---------------------------------*/
/**
 * @brief 의존성그래프 출력
 *
 * @param tn 출력을 시작할 루트노드
 */
/* ---------------------------------*/
void print_tree(TNode *tn)
{
	int child = (int)tn->child_cnt;
	//현재타겟 출력
	printf("%s",((Block*)tn->item)->target);
	//출력한 만큼의 공백을 리스트에 추가
	addNode(&indent, tospace(((Block*)tn->item)->target));
	Node *circ = searchList(&printed,((Block*)tn->item)->target,compstr);
	if (circ == NULL)
		addNode(&printed, ((Block*)tn->item)->target);
	for (int i=0;circ==NULL&&i<child;++i)
	{
		//세로줄 또는 공백
		if (i==child-1)
			addNode(&indent, " ");
		else
			addNode(&indent, ud);

		if (i<=child-2)
		{
			if (i==0)
				printf("%s",lrd);
			else if (i>=1)
				printf("%s",rud);
		}
		else if (child==1)
			printf("%s",lr);
		else if (i==child-1)
			printf("%s",ru);
		print_tree(tn->child[i]);
		//인덴트리스트 출력
		if (i<child-1)
			for (indent.cur=indent.head;
					indent.cur!=NULL;
					indent.cur=indent.cur->next)
				printf("%s",(char*)indent.cur->item);
	}
	delNode(&indent);
	delNode(&indent);
	if (circ == NULL && child == 0)
		printf("\n");
}

void print_err()					//에러
{
	printf("ERROR!\n");
}

void print_war()					//경고
{
}

void print_new(const char *name)	//실행불필요시
{
	printf("ssu_make: '%s'is up to date.\n",name);
}
