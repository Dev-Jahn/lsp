#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <unistd.h>
#include "constants.h"

typedef struct _Node
{
	void *item;
	struct _Node *next;
} Node;

typedef struct _List
{
	Node *cur;
	Node *head;
	Node *tail;
	size_t size;
} List;

typedef struct _Stack
{
	Node *top;
	int size;
} Stack;

typedef struct _Pair
{
	char *key;
	char *value;
} Pair;

typedef struct _Block
{
	char *target;
	char **depends;
	size_t dep_cnt;
	char **cmds;
	size_t cmd_cnt;
} Block;

typedef struct _TreeNode
{
	void *item;
	size_t child_cnt;
	struct _TreeNode **child;
} TNode;

typedef struct _Tree
{
	TNode *root;
} Tree;

typedef struct _Command
{
	Pair opt[MAX_OPTION];
	char* target[MAX_TARGET];
	Pair macro[MAX_MACRO];
} Command;

typedef struct _Off_Pair
{
	int found;
	off_t so;
	off_t eo;
} Off_Pair;

void initList(List *list);
void addNode(List *list, void *item);
void delNode(List *list);
void clearList(List *list);
Node *searchList(List *list, void *item, int (*cmp)(const void*, const void*));
void initStack(Stack *stk);
void push(Stack *stk, void *data);
void *pop(Stack *stk);
Block *newBlock(char *target);
void addDepend(Block *blk, char *depend);
void addCmd(Block *blk, char *cmd);
void freeBlock(Block *blk);
void initTree(Tree *tree, void *rootItem);
void addChild(TNode *root, void *item);
Pair *newPair(char *k, char *v);
void freePair(Pair *p);
int compKey(const void *a, const void *b);
int compVal(const void *a, const void *b);

#endif
