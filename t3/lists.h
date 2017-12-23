/**
	>>>> LISTS <<<<
	Estruturas e funcoes para uso
	de listas ligadas.
**/

#ifndef _LISTS_H_
#define _LISTS_H_

#include "trees.h"

typedef struct sListNode ListNode;
typedef struct sList List;
typedef TreeNode ListItem;

struct sListNode {
	ListItem *item;
	ListNode *next;
	ListNode *prev;
};

struct sList {
	ListNode *first;
	ListNode *last;
	int size;
};

List *listCreate();
ListNode *listCreateNode(ListItem *it);
int listGetSize(List *list);
void listPrintNode(ListNode *n);
void listPrint(List *list);
void listOrdInsert(List *list, ListItem *it);
ListItem *listRemoveFirst(List *list);
void listFree(List *list);

#endif
