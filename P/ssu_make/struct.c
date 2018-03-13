#include "struct.h"

LIST *makeList()
{
	LIST* newlist = (LIST*)malloc(sizeof(LIST));
	newlist->cur = NULL;
	newlist->head = NULL;
	newlist->tail = NULL;
	return newlist;
}

void addNode(LIST *list, char *input)
{
	NODE* nd = (NODE*)malloc(sizeof(NODE));
	nd->str = input;
	nd->next = NULL;
	if ((list->head==NULL)&&(list->tail==NULL))
		list->head = list->tail = nd;
	else
	{
		list->tail->next = nd;
		list->tail = nd;
	}
	list->cur = nd;
}

void delNode(LIST *list)
{
	NODE* nd = list->head;
	if (nd==list->tail)
	{
		free(nd);
		list->head = list->tail = list->cur = NULL;
		return;
	}
	while(nd->next->next != NULL)
		nd = nd->next;
	free(nd->next);
	nd->next = NULL;
	list->tail = nd;
}

void clearList(LIST *list)
{
	while (list->head!=NULL)
		delNode(list);
}

void init(STACK *stk)
{
	stk->top = NULL;
	stk->size = 0;
}

void push(STACK *stk, char *data)
{
	NODE *nd = (NODE*)malloc(sizeof(NODE));
	nd->str = data;
	nd->next = stk->top;
	stk->top = nd;
	(stk->size)++;
}

char *pop(STACK *stk)
{
	if ((stk->size)==0)
		return NULL;
	else
	{
		char *data = (char*)malloc(sizeof(char));
		NODE *old = stk->top;
		data = old->str;
		stk->top = old->next;
		free(old);
		return data;
	}
}
