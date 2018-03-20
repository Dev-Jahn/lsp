#include <stdlib.h>
#include <string.h>
#include "struct.h"

/*
List *makeList()
{
	List* newList = (List*)malloc(sizeof(List));
	newList->cur = NULL;
	newList->head = NULL;
	newList->tail = NULL;
	return newList;
}

void addNode(List *List, char *input)
{
	Node* nd = (Node*)malloc(sizeof(Node));
	nd->str = input;
	nd->next = NULL;
	if ((List->head==NULL)&&(List->tail==NULL))
		List->head = List->tail = nd;
	else
	{
		List->tail->next = nd;
		List->tail = nd;
	}
	List->cur = nd;
}

void delNode(List *List)
{
	Node* nd = List->head;
	if (nd==List->tail)
	{
		free(nd);
		List->head = List->tail = List->cur = NULL;
		return;
	}
	while(nd->next->next != NULL)
		nd = nd->next;
	free(nd->next);
	nd->next = NULL;
	List->tail = nd;
}

void clearList(List *List)
{
	while (List->head!=NULL)
		delNode(List);
}
*/
void init(Stack *stk)
{
	stk->top = NULL;
	stk->size = 0;
}

void push(Stack *stk, char *data)
{
	Node *nd = (Node*)malloc(sizeof(Node));
	nd->str = data;
	nd->next = stk->top;
	stk->top = nd;
	(stk->size)++;
}

char *pop(Stack *stk)
{
	if ((stk->size)==0)
		return NULL;
	else
	{
		(stk->size)--;
		Node *old = stk->top;
		int len = strlen(old->str);
		char *data = (char*)malloc(sizeof(char)*len);
		memcpy(data, old->str, len);
		stk->top = old->next;
		free(old);
		return data;
	}
}

TNode *newNode(Block blk)
{
	TNode *ret = (TNode *)malloc(sizeof(TNode));
	ret->blk = blk;
	ret->child_cnt = 0;
	ret->child = NULL;
	return ret;
}

void addChild(TNode *root, TNode *child)
{
	TNode **p = (TNode**)malloc(sizeof(TNode*)*(root->child_cnt+1));
	memcpy(p, root->child, sizeof(TNode*)*(root->child_cnt));
	p[root->child_cnt] = child;
	root->child = p;
}

Tree *newTree()
{
	Tree *ret = (Tree*)malloc(sizeof(Tree));
	ret->root = NULL;
	return ret;
}

