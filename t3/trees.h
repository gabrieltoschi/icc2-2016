/**
	>>>> TREES <<<<
	Estruturas e funcoes para uso
	de arvores binarias.
**/

#ifndef _TREES_H_
#define _TREES_H_

#include "tools.h"

typedef struct sTreeNode TreeNode;
typedef struct sTree Tree;
typedef Huff TreeItem;

struct sTreeNode {
	TreeItem *item;
	TreeNode *left;
	TreeNode *right;
};

struct sTree {
	TreeNode *root;
};

Tree *treeCreate();
TreeNode *treeCreateNode(TreeItem *it);
void treeAssemblyNode(TreeNode *root, TreeNode *l, TreeNode *r);
void treePrintNode(TreeNode *n);
void treePrint(Tree *t);
void treeAssemblyRoot(Tree *t, TreeNode *root);
void treeDestroy(Tree *t);
	
#endif
