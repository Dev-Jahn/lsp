#include <stdlib.h>
#include <string.h>
#include "struct.h"

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

void delNode(List *list)
{
	Node* nd = list->head;
	if (nd==list->tail)
	{
		free(nd);
		list->head = list->tail = list->cur = NULL;
		return;
	}
	while(nd->next->next != NULL)
		nd = nd->next;
	free(nd->next->item);
	free(nd->next);
	nd->next = NULL;
	list->tail = nd;
	list->size--;
}

void clearList(List *list)
{
	while (list->head!=NULL)
		delNode(list);
}

void initStack(Stack *stk)
{
	stk->top = NULL;
	stk->size = 0;
}

void push(Stack *stk, void *data)
{
	Node *nd = (Node*)malloc(sizeof(Node));
	nd->item = data;
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
 * @return top노드안에 들어있던 문자열.해제필요
 */
/* ---------------------------------*/
void *pop(Stack *stk)
{
	if ((stk->size)==0)
		return NULL;
	else
	{
		(stk->size)--;
		Node *old = stk->top;
		void *item = old->item;
		stk->top = old->next;
		free(old);
		return item;
	}
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

void addChild(TNode *root, void *item)
{
	TNode **p = (TNode**)malloc(sizeof(TNode*)*(root->child_cnt+1));
	TNode *child = (TNode*)malloc(sizeof(TNode));
	child->item = item;
	child->child_cnt = 0;
	child->child = NULL;
	if (root->child != NULL)
	{
		memcpy(p, root->child, sizeof(TNode*)*(root->child_cnt));
		free(root->child);
	}
	p[root->child_cnt] = child;
	root->child = p;
	root->child_cnt++;
}
