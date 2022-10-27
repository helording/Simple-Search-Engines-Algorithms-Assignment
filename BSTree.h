// Binary Search Tree ADT interface ... COMP2521 


#ifndef BSTREE_H
#define BSTREE_H

#include <stdbool.h>
#include "set.h"

/*
#define RANDOM_ROOT_INSERT (random() % 10 < 4)   // 40% chance

#define data(tree)  ((tree)->data)
#define urls(tree)  ((tree)->urls->elems)
#define left(tree)  ((tree)->left)
#define right(tree)  ((tree)->right)
typedef int Item;      // item is just a key

typedef struct tNode *Tree;

typedef struct tNode {
   char  *data;
   Set urls;
   Tree left, right;
} tNode;

*/


#define data(tree)  ((tree)->data)
#define urls(tree)  ((tree)->urls->elems)
#define left(tree)  ((tree)->left)
#define right(tree)  ((tree)->right)

typedef char *Item; 

typedef struct tNode *Tree;

typedef struct tNode {
   char  *data;
   Set urls;
   Tree left, right;
} tNode;



Tree newTree();        // create an empty Tree
void freeTree(Tree);   // free memory associated with Tree
void showTree(Tree);   // display a Tree (sideways)

bool TreeSearch(Tree, Item);   // check whether an item is in a Tree
int  TreeHeight(Tree);         // compute height of Tree
int  TreeNumtNodes(Tree);       // count #tNodes in Tree
Tree TreeInsert(Tree, Item, char *);   // insert a new item into a Tree
Tree TreeDelete(Tree, Item);   // delete an item from a Tree

// internal functions made visible for testing
Tree rotateRight(Tree);
Tree rotateLeft(Tree);
Tree insertAtRoot(Tree, Item);
Tree insertRandom(Tree, Item, char *);
Tree insertAVL(Tree, Item);
Tree partition(Tree, int);
Tree rebalance(Tree);

#endif
