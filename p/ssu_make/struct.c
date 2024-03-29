#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

Queue q;

void initList(List *list)
{
	list->cur = NULL;
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

void addNode(List *list, void *item)
{
	Node* nd = (Node*)malloc(sizeof(Node));
	nd->item = item;
	nd->next = NULL;
	if ((list->head==NULL)&&(list->tail==NULL))
	{
		list->head = list->tail = nd;
		list->cur = list->head;
	}
	else
	{
		list->tail->next = nd;
		list->tail = nd;
	}
	list->size++;
}

/* ---------------------------------*/
/**
 * @brief 
 *
 * @param list
 */
/* ---------------------------------*/
void delNode(List *list)
{
	Node* nd = list->head;
	if (nd==NULL)
		return;
	if (nd==list->tail)
	{
		/*free(nd);*/
		list->head = list->tail = list->cur = NULL;
		list->size = 0;
		return;
	}
	while(nd->next->next != NULL)
		nd = nd->next;
	/*
	 *free(nd->next->item);
	 *free(nd->next);
	 */
	nd->next = NULL;
	list->tail = nd;
	list->size--;
}

void clearList(List *list)
{
	while (list->head!=NULL)
		delNode(list);
}

Node *searchList(List *list, void *item, int (*cmp)(const void*, const void*))
{
	list->cur = list->head;
	while(list->cur!=NULL)
	{
		if (cmp(list->cur->item, item) == 0)
			break;
		list->cur = list->cur->next;
	}
	return list->cur;
}

int compstr(const void *a, const void *b)
{
	return strcmp((char*)a, (char*)b);
}

void initStack(Stack *stk)
{
	stk->top = NULL;
	stk->size = 0;
}

void push(Stack *stk, void *item)
{
	Node *nd = (Node*)malloc(sizeof(Node));
	nd->item = item;
	nd->next = stk->top;
	stk->top = nd;
	(stk->size)++;
}

/* ---------------------------------*/
/**
 * @brief 스택에서 top노드를 꺼내는 함수
 *
 * @param stk 스택을 가리키는 포인터
 *
 * @return top노드안에 들어있던 원소.해제필요
 */
/* ---------------------------------*/
void *pop(Stack *stk)
{
	if ((stk->size)==0)
		return NULL;
	else
	{
		Node *old = stk->top;
		void *item = old->item;
		stk->top = old->next;
		free(old);
		(stk->size)--;
		return item;
	}
}

void initQueue(Queue *q)
{
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
}

void enqueue(Queue *q, void *item)
{
	Node *nd = (Node*)malloc(sizeof(Node));
	nd->item = item;
	nd->next = NULL;
	if ((q->head==NULL)&&(q->tail==NULL))
		q->head = q->tail = nd;
	else
	{
		q->tail->next = nd;
		q->tail = nd;
	}
	q->size++;
}

void *dequeue(Queue *q)
{
	if ((q->size)==0)
		return NULL;
	else
	{
		void *item = (TNode*)(q->head->item);
		Node *old = q->head;
		q->head = old->next;
		if (q->head == NULL)
			q->tail = NULL;
		free(old);
		(q->size)--;
		return item;
	}
}

Block *newBlock(const char *target)
{
	Block *blk = (Block*)malloc(sizeof(Block));
	char *newtar = (char*)malloc(sizeof(char)*(strlen(target)+1));
	memcpy(newtar, target, sizeof(char)*(strlen(target)+1));
	blk->target = newtar;
	blk->depends= NULL;
	blk->dep_cnt= 0;
	blk->cmds = NULL;
	blk->cmd_cnt = 0;
	return blk;
}

void addDepend(Block *blk, char *depend)
{
	char *copy = (char*)malloc(sizeof(char)*(strlen(depend)+1));
	memcpy(copy, depend, sizeof(char)*(strlen(depend)+1));
	char **newdep = (char**)malloc(sizeof(char*)*(blk->dep_cnt+1));
	memcpy(newdep, blk->depends, sizeof(char*)*(blk->dep_cnt));
	newdep[blk->dep_cnt] = copy;
	free(blk->depends);
	blk->depends = newdep;
	blk->dep_cnt++;
}

void addCmd(Block *blk, char *cmd)
{
	char *copy = (char*)malloc(sizeof(char)*(strlen(cmd)+1));
	memcpy(copy, cmd, sizeof(char)*(strlen(cmd)+1));
	char **newcmds= (char**)malloc(sizeof(char*)*(blk->cmd_cnt+1));
	memcpy(newcmds, blk->cmds, sizeof(char*)*(blk->cmd_cnt));
	newcmds[blk->cmd_cnt] = copy;
	free(blk->cmds);
	blk->cmds= newcmds;
	blk->cmd_cnt++;
}

void freeBlock(Block *blk)
{
	free(blk->target);
	while(blk->dep_cnt)
	{
		free(blk->depends[blk->dep_cnt-1]);
		blk->dep_cnt--;
	}
	while(blk->cmd_cnt)
	{
		free(blk->cmds[blk->cmd_cnt-1]);
		blk->cmd_cnt--;
	}
	free(blk->depends);
	free(blk->cmds);
}

void initTree(Tree *tree, void *rootItem)
{
	tree->root = (TNode*)malloc(sizeof(TNode));
	tree->root->child = NULL;
	tree->root->child_cnt = 0;
	tree->root->item = rootItem;
}

TNode *newNode(void *item)
{
	TNode *ret = (TNode *)malloc(sizeof(TNode));
	ret->item = item;
	ret->child_cnt = 0;
	ret->child = NULL;
	return ret;
}

TNode *addChild(TNode *root, void *item)
{
	TNode **arr = (TNode**)malloc(sizeof(TNode*)*(root->child_cnt+1));
	TNode *child = (TNode*)malloc(sizeof(TNode));
	child->item = item;
	child->child_cnt = 0;
	child->child = NULL;
	if (root->child != NULL)
	{
		memcpy(arr, root->child, sizeof(TNode*)*(root->child_cnt));
		free(root->child);
	}
	arr[root->child_cnt] = child;
	root->child = arr;
	root->child_cnt++;
	return child;
}

TNode *linkChild(TNode *root, TNode *child)
{
	TNode **arr = (TNode**)malloc(sizeof(TNode*)*(root->child_cnt+1));
	if (root->child != NULL)
	{
		memcpy(arr, root->child, sizeof(TNode*)*(root->child_cnt));
		free(root->child);
	}
	arr[root->child_cnt] = child;
	root->child = arr;
	root->child_cnt++;
	addDepend(root->item,((Block*)child->item)->target);
	return child;
}

int countleaf(TNode *root)
{
	int leaf = 0;
	if (root == NULL)
		return 0;
	for (int i=0;i<(int)root->child_cnt;++i)
		leaf+=countleaf(root->child[i]);
	if (root->child_cnt == 0)
		leaf = 1;
	return leaf;
}

/* ---------------------------------*/
/**
 * @brief 타겟이름을 검색하는 함수(용도한정)
 *
 * @param tn 검색을 시작할 루트노드
 * @param target 
 *
 * @return 찾으면 해당노드포인터, 찾지 못하거나 인자가 NULL이면 NULL.
 */
/* ---------------------------------*/
TNode *dfstarget(TNode *tn, char *target)
{
	if (tn == NULL)
		return NULL;
	TNode *found = NULL;
	if (strcmp(((Block*)tn->item)->target, target) == 0)
		return tn;
	for (int i=0;i<(int)tn->child_cnt && found==NULL;++i)
		found = dfstarget(tn->child[i], target);
	return found;
}

TNode *bfstarget(TNode *tn, char *target)
{
	if (tn == NULL)
		return NULL;
	TNode *found = NULL;
	if (strcmp(((Block*)tn->item)->target, target) == 0)
		return tn;
	for (int i=0;i<(int)tn->child_cnt;++i)
	{
		if (strcmp(((Block*)tn->child[i]->item)->target, target) == 0)
			return tn->child[i];
		enqueue(&q,tn->child[i]);
	}
	found = bfstarget((TNode*)(dequeue(&q)),target);
	return found;
}

Pair *newPair(char *k, char *v)
{
	Pair *p = (Pair*)malloc(sizeof(Pair));
	p->key = (char*)malloc(strlen(k)+1);
	memcpy(p->key,k,strlen(k)+1);
	p->value = (char*)malloc(strlen(v)+1);
	memcpy(p->value, v, strlen(v)+1);
	return p;
}

void freePair(Pair *p)
{
	free(p->key);
	free(p->value);
	free(p);
}

int compKey(const void *a, const void *b)
{
	return strcmp(((Pair*)a)->key,((Pair*)b)->key);
}

int compVal(const void *a, const void *b)
{
	return strcmp(((Pair*)a)->value,((Pair*)b)->value);
}

int compPtr(const void *a, const void *b)
{
	if (a==b)
		return 0;
	else
		return -1;
}
