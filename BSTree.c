// Binary Search Tree ADT implementation ... COMP2521 
// Modifed ADT from Week 8 : Exercises BSTree.c and BSTree.h
#include "BSTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "set.h"

// make a new tNode containing data
Tree newtNode(char *it) {
   Tree new = malloc(sizeof(tNode));
   assert(new != NULL);
   char *newSpace = malloc(strlen(it) + 1);
   assert(newSpace != NULL);
   strcpy(newSpace, it);
   data(new) = newSpace;
   new->urls = newSet();
   left(new) = right(new) = NULL;
   return new;
}

// create a new empty Tree
Tree newTree() {
   return NULL;
}

// free memory associated with Tree
void freeTree(Tree t) {
   if (t != NULL) {
      freeTree(left(t));
      freeTree(right(t));
      disposeSet(t->urls);
      free(t->data);
      free(t);
   }
}

// display Tree horizontally
// code by Jin Qu


// compute height of Tree
int TreeHeight(Tree t) {
   if (t == NULL) {
      return 0;
   } else {
      int lheight = 1 + TreeHeight(left(t));
      int rheight = 1 + TreeHeight(right(t));
      if (lheight > rheight)
	 return lheight;
      else
	 return rheight;
   }
}

// count #tNodes in Tree
int TreeNumtNodes(Tree t) {
   if (t == NULL)
      return 0;
   else
      return 1 + TreeNumtNodes(left(t)) + TreeNumtNodes(right(t));
}

// check whether a key is in a Tree
bool TreeSearch(Tree t, char *it) {
   if (t == NULL)
      return false;
   else if (strLT(data(t), it))
      return TreeSearch(left(t), it);
   else if (strLT(it, data(t)))
      return TreeSearch(right(t), it);
   else                                 // it == data(t)
      return true;
}

// insert a new item into a Tree
Tree TreeInsert(Tree t, Item it, char *url) {
   //if (t != NULL) printf("data(t) = %s\n", data(t));
   
   if (t == NULL) {
    //  printf("making new node\n");
      t = newtNode(it);
      insertInto(t->urls, url);
    } 
   else if (strcmp(data(t),it)==0) {
//	printf("%s %s\n", data(t), it);
	
   	insertInto(t->urls, url);
   } 
   else if (strLT(it, data(t))){
    //  printf("left\n");
      left(t) = TreeInsert(left(t), it, url);
   }
   else if (strLT(data(t), it)){
    //  printf("right\n");
      right(t) = TreeInsert(right(t), it, url);
   }
     
   return t;
}

Tree joinTrees(Tree t1, Tree t2) {
   if (t1 == NULL)
      return t1;
   else if (t2 == NULL)
      return t2;
   else {
      Tree curr = t2;
      Tree parent = NULL;
      while (left(curr) != NULL) {    // find min element in t2
	 parent = curr;
	 curr = left(curr);
      }
      if (parent != NULL) {
	 left(parent) = right(curr);  // unlink min element from parent
	 right(curr) = t2;
      }
      left(curr) = t1;
      return curr;                    // min element is new root
   }
}

// delete an item from a Tree
Tree TreeDelete(Tree t, Item it) {
   if (t != NULL) {
      if (strLT(data(t), it))
	 left(t) = TreeDelete(left(t), it);
      else if (strLT(it, data(t)))
	 right(t) = TreeDelete(right(t), it);
      else {
	 Tree new;
	 if (left(t) == NULL && right(t) == NULL) 
	    new = NULL;
	 else if (left(t) == NULL)    // if only right subtree, make it the new root
	    new = right(t);
	 else if (right(t) == NULL)   // if only left subtree, make it the new root
	    new = left(t);
	 else                         // left(t) != NULL and right(t) != NULL
	    new = joinTrees(left(t), right(t));
	 free(t);
	 t = new;
      }
   }
   return t;
}

Tree rotateRight(Tree n1) {
   if (n1 == NULL || left(n1) == NULL)
      return n1;
   Tree n2 = left(n1);
   left(n1) = right(n2);
   right(n2) = n1;
   return n2;
}

Tree rotateLeft(Tree n2) {
   if (n2 == NULL || right(n2) == NULL)
      return n2;
   Tree n1 = right(n2);
   right(n2) = left(n1);
   left(n1) = n2;
   return n1;
}

Tree insertAtRoot(Tree t, Item it) { 
   if (t == NULL) {
      t = newtNode(it);
   } else if (strLT(data(t), it)) {
      left(t) = insertAtRoot(left(t), it);
      t = rotateRight(t);
   } else if (strLT(it, data(t))) {
      right(t) = insertAtRoot(right(t), it);
      t = rotateLeft(t);
   }
   return t;
}



Tree partition(Tree t, int i) {
   if (t != NULL) {
      assert(0 <= i && i < TreeNumtNodes(t));
      int m = TreeNumtNodes(left(t));
      if (i < m) {
	 left(t) = partition(left(t), i);
	 t = rotateRight(t);
      } else if (i > m) {
	 right(t) = partition(right(t), i-m-1);
	 t = rotateLeft(t);
      }
   }
   return t;
}

Tree rebalance(Tree t) {
   int n = TreeNumtNodes(t);
   if (n >= 3) {
      t = partition(t, n/2);           // put tNode with median key at root
      left(t) = rebalance(left(t));    // then rebalance each subtree
      right(t) = rebalance(right(t));
   }
   return t;
}


