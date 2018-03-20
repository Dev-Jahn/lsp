#ifndef _STRUCT_H_
#define _STRUCT_H_

#define MAX_OPTION 6
#define MAX_TARGET 5
#define MAX_MACRO 5

typedef struct _Node
{
	char *str;
	struct _Node *next;
} Node;

typedef struct _Block
{
	char *key;
	char **val;
	int val_cnt;
	char **cmd;
	int cmd_cnt;
} Block;

typedef struct _TreeNode
{
	Block blk;
	int child_cnt;
	struct _TreeNode **child;
} TNode;

typedef struct _Tree
{
	TNode *root;
} Tree;

/*
 *typedef struct _List
 *{
 *    Node *cur;
 *    Node *head;
 *    Node *tail;
 *} List;
 */

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

typedef struct _Command
{
	Pair opt[MAX_OPTION];
	char* target[MAX_TARGET];
	Pair macro[MAX_MACRO];
} Command;

//List *makeList();
//void addNode(List *List, char *input);
//void delNode(List *List);
//void clearList(List *List);
void init(Stack *stk);
void push(Stack *stk, char *data);
char *pop(Stack *stk);
TNode *newNode(Block blk);
void addChild(TNode *root, TNode *child);
Tree *newTree();

#endif
