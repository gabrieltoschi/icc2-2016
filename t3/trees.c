/**
	>>>> TREES <<<<
	Estruturas e funcoes para uso
	de arvores binarias.
**/

#include "trees.h"
#include "huffman.h"

#include <stdlib.h>
#include <stdio.h>

/** >> treeCreate
	Inicializa uma arvore.
**/
Tree *treeCreate(){
	Tree *t = (Tree *) malloc(sizeof(Tree));
	t->root = NULL;
	
	return t;
}

/** >> treeCreateNode
	Inicializa um no de arvore.
**/
TreeNode *treeCreateNode(TreeItem *it){
	TreeNode *node = (TreeNode *) malloc(sizeof(TreeNode));
	node->item = it;
	node->left = NULL;
	node->right = NULL;
	
	return node;
}

/** >> treeAssemblyNode
	Acopla dois nos filhos a um no raiz.
**/
void treeAssemblyNode(TreeNode *root, TreeNode *l, TreeNode *r){
	root->left = l;
	root->right = r;
}

/** >> treePrintNode
	Imprime um no de arvore na saida padrao.
**/
void treePrintNode(TreeNode *n){
	printHuff(n->item);
}

/** >> _listPrint
	Imprime recursivamente uma lista "em ordem".
**/
void _treePrint(TreeNode *n){
	if (n == NULL) return;
	_treePrint(n->left);
	treePrintNode(n);
	_treePrint(n->right);
}

/** >> listPrint
	Inicia o processo recursivo de impressao
	de uma arvore.
**/
void treePrint(Tree *t){
	_treePrint(t->root);
}

/** >> treeAssemblyRoot
	Acopla um no como raiz de uma arvore.
**/
void treeAssemblyRoot(Tree *t, TreeNode *root){
	t->root = root;
}

/** >> treeNodeDestroy
	Libera recursivamente os nos de uma arvore.
**/
void treeNodeDestroy(TreeNode *n){
	if (n->left != NULL) treeNodeDestroy(n->left);
	if (n->right != NULL) treeNodeDestroy(n->right);
	
	freeHuff(n->item);
	free(n);
}

/** >> treeDestroy
	Libera uma arvore alocado na memoria Heap.
**/
void treeDestroy(Tree *t){
	treeNodeDestroy(t->root);
	free(t);
}


