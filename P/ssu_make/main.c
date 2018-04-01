#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "core.h"
#include "util.h"
#include "pp.h"
#include "parser.h"
#include "main.h"
#include "struct.h"
#include "patterns.h"
#include "ui.h"

int flag = 000;
char *filename;
char *directory;
List execed;
Tree parsetree;

void treetest(TNode *tn)
{
	printf("%s",((Block*)tn->item)->target);
	/*printf("child:%ld\n",tn->child_cnt);*/
	for (int i=0;i<(int)(tn->child_cnt);i++)
	{
		treetest(tn->child[i]);
	}
	printf("out\n");
}

int main(int argc, char *argv[])
{
	filename = "Makefile";
	directory = ".";


	
	int argoff = setopt(argc, argv);
	if (chdir(directory)<0)
	{
		fprintf(stderr, "ssu_make: There's no such directory.\n");
		exit(1);
	}

	List targets, macros;
	initList(&targets);
	initList(&macros);
	initList(&execed);
	for (int i=argoff; i<argc; ++i)
	{
		if (compare("\\w+", argv[i]) == 0)
			addNode(&targets, argv[i]);
		else if(compare(pat_essential, argv[i]) == 0)
		{
			char *k, *v, *tmp = (char*)malloc(strlen(argv[i])+1);
			strcpy(tmp, argv[i]);
			k = strtok(tmp, "=");
			v = strtok(NULL, "=");
			addNode(&macros, newPair(k,v));
		}
		else
			print_err();
	}
	char tmpfile[WORD_SIZE];
	strcpy(tmpfile,filename);
	strcat(tmpfile,".tmp");
	int fd;
	initTree(&parsetree, NULL);
	fd = preprocess(filename, &macros);
	if (ON_P(flag))
	{
		printf("ssu_make: Preprocessed file \"%s\" generated.\n",tmpfile);
		exit(0);
	}
	initList(&entered);
	parse_Tree(fd, NULL, parsetree.root);
	close(fd);
	if (remove(tmpfile)<0)
		fprintf(stderr, "ssu_make: Can't remove %s\n",tmpfile);
	checkopt();
	//명령행에서 입력한 타겟 존재
	if (targets.size != 0 )
	{
		for (targets.cur = targets.head;
				targets.cur!=NULL;
				targets.cur=targets.cur->next)
		{
			TNode *tn = NULL;
			//이미 실행한 타겟 리스트에서 원소마다 dfs실시
			for (execed.cur = execed.head;
					execed.cur!=NULL;
					execed.cur=execed.cur->next)
			{
				tn = dfstarget((TNode*)execed.cur->item,(char*)targets.cur->item);
				if (tn != NULL)
					break;
			}
			if (tn != NULL)
				break;
			tn = dfstarget(parsetree.root,(char*)targets.cur->item);
			if (tn == NULL)
			{
				fprintf(stderr,"ssu_make: Can't find target \"%s\".\n",(char*)targets.cur->item);
				exit(1);
			}

			if (newest(tn, 0) == 1)
				print_new((char*)targets.cur->item);
			else
				execute(tn);
		}
	}
	//기본값. 가장 위 타겟
	if (parsetree.root->child_cnt == 0)
	{
		fprintf(stderr,"ssu_make: There is no target in the file.\n");
		exit(1);
	}
	else if(newest(parsetree.root->child[0],0) == 1)
		print_new(((Block*)parsetree.root->child[0]->item)->target);
	else
		execute(parsetree.root->child[0]);
	exit(0);
}

/* ---------------------------------*/
/**
 * @brief 
 *
 * @param argc
 * @param argv[]
 *
 * @return 옵션이 끝난 이후의 인덱스 리턴
 */
/* ---------------------------------*/
int setopt(int argc, char * const argv[])
{
	int c;
	while ((c=getopt(argc, argv, "mhstpDf:c:")) != -1)
	{
		switch(c)
		{
			case 'm':
				flag = flag|OPT_M;
				break;
			case 'h':
				flag = flag|OPT_H;
				break;
			case 's':
				flag = flag|OPT_S;
				break;
			case 't':
				flag = flag|OPT_T;
				break;
			case 'f':
				flag = flag|OPT_F;
				filename = optarg;
				break;
			case 'c':
				flag = flag|OPT_C;
				directory = optarg;
				break;
			case 'p':
				flag = flag|OPT_P;
				break;
			case 'D':
				flag = flag|OPT_D;
				break;
		}
	}
	return optind;
}

void checkopt()
{
	if (ON_H(flag))
	{
		print_help();
		exit(0);
	}
	if (ON_M(flag))
		print_mac(&macroList);
	if (ON_T(flag))
	{
		initList(&indent);
		initList(&printed);
		print_tree(parsetree.root);
	}
	if (ON_M(flag))
		exit(0);
}
