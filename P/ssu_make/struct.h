#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <stdlib.h>

#define MAX_OPTION 6
#define MAX_TARGET 5
#define MAX_MACRO 5

typedef struct _NODE
{
	char *str;
	struct _NODE *next;
} NODE;

typedef struct _LIST
{
	NODE *cur;
	NODE *head;
	NODE *tail;
} LIST;

typedef struct _STACK
{
	NODE *top;
	int size;
} STACK;

typedef struct _PAIR
{
	char *key;
	char *value;
} PAIR;

typedef struct _COMMAND
{
	PAIR opt[MAX_OPTION];
	char* target[MAX_TARGET];
	PAIR macro[MAX_MACRO];
} COMMAND;

LIST *makeList();
void addNode(LIST *list, char *input);
void delNode(LIST *list);
void clearList(LIST *list);
void init(STACK *stk);
void push(STACK *stk, char *data);
char *pop(STACK *stk);

#endif
