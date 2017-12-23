/**
	>>>> LISTS <<<<
	Estruturas e funcoes para uso
	de listas ligadas.
**/

#include "lists.h"
#include "trees.h"

#include <stdlib.h>
#include <stdio.h>

/** >> listCreate
	Inicializa uma lista.
**/
List *listCreate(){
	List *l = (List *) malloc(sizeof(List));
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
	
	return l;
}

/** >> listCreateNode
	Inicializa um no de lista a
	partir de um item.
**/
ListNode *listCreateNode(ListItem *it){
	ListNode *n = (ListNode *) malloc(sizeof(ListNode));
	n->next = NULL;
	n->prev = NULL;
	n->item = it;
	
	return n;
}

/** >> listGetSize
	Retorna o tamanho de uma lista.
**/
int listGetSize(List *list){
	return list->size;
}

/** >> listPrintNode
	Imprime um no de lista na saida padrao.
**/
void listPrintNode(ListNode *n){
	treePrintNode(n->item);
}

/** >> _listPrint
	Imprime recursivamente uma lista.
**/
void _listPrint(ListNode *n){
	if (n == NULL) return;
	listPrintNode(n);
	_listPrint(n->next);
}

/** >> listPrint
	Inicia o processo recursivo de impressao
	de uma lista.
**/
void listPrint(List *list){
	_listPrint(list->first);
}

/** >> listOrdInsert
	Insere um novo elemento na lista de forma
	ordenada.
**/
void listOrdInsert(List *list, ListItem *it){
	ListNode *new, *p = list->first;
	
	while (p != NULL){
		if (it->item->frequence > p->item->item->frequence)
			p = p->next;
		else if (it->item->frequence == p->item->item->frequence && it->item->string[0] > p->item->item->string[0])
			p = p->next;
		else break;
	}
	
	new = listCreateNode(it);
	list->size++;
	
	if (list->size == 1){ // esse e' o primeiro no da lista
		list->first = new;
		list->last = new;
		return;
	} 
	
	if (p == list->first){ // novo item deve ser colocado no começo da lista
		new->prev = NULL;
		new->next = list->first;
		list->first->prev = new;
		list->first = new;
		return;
	}
	
	if (p == NULL){ // novo item deve ser colocado ao final da lista
		new->next = NULL;
		new->prev = list->last;
		list->last->next = new;
		list->last = new;
		return; 
	}
	
	// novo item deve ser colocado no meio da lista
	new->next = p;
	new->prev = p->prev;
	p->prev->next = new;
	p->prev = new;
	return;
}

/** >> listRemoveFirst
	Remove e retorna o primeiro no da lista.
**/
ListItem *listRemoveFirst(List *list){
	ListNode *rm = list->first;
	ListItem *it = rm->item;
	
	if (list->size == 2) {
		list->first = list->first->next;
		list->first->prev = NULL;
	} else if (list->size == 1) {
		list->first = NULL;
		list->last = NULL;
	} else {
		list->first->next->prev = NULL;
		list->first = list->first->next;
	}
		
	list->size--;
	free(rm);
	
	return it;
}

/** >> listFree
	Remove todos os nos de uma lista
	e a libera da memoria Heap.
**/
void listFree(List *list){
	while (list->size > 0){
		listRemoveFirst(list);
	}
	
	free(list);
}

