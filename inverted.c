// libraries 
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "set.h"
#include "graph.h"
#include "readData.h"
#include "BSTree.h"
#include <ctype.h> 
#include "inverted.h"


#define MAX_L 2000 



int main(void){

	// opens collection, gets urls and creates a word BST from urls. Analyses the tree and opens the analysis to invertedIndex.txt
	FILE *fp = openFile("collection");
	
	Set urls = GetCollection(fp);

	Tree t = createWordTree(urls);

	createII(t);
	
	// closes and frees
    	fclose(fp);
	freeTree(t);
	disposeSet(urls);

	return 0;

}

// creates a word BST from all the urls scanned from collection
Tree createWordTree(Set urls){
	int start_reading = 0;
	Link curr = urls->elems;
	char scanned[MAX_L];
	Tree t = newTree();
	char *nScanned;

	// while there are urls, scan there section 2 contents and add them to the tree
	while (curr != NULL) {
		FILE *fp = openFile(curr->val);
		start_reading = 0;
		while ((fscanf(fp, "%s", scanned)) != EOF){
			if (strcmp(scanned, "Section-2") != 0 && start_reading != 1){
				continue;
			} else if (strcmp(scanned, "Section-2") == 0) {
				//printf("..\n");
				start_reading = 1;
				continue;
				
			}

			if ((strcmp(scanned, "#end")) == 0) break;

			nScanned = normalise(scanned);

			t = TreeInsert(t, nScanned, curr->val);
			free(nScanned);
		}
		fclose(fp);
		curr = curr->next;
	}
	return t;
}

// First create inverted index function. Used to open the file
void createII(Tree t){
	FILE *fp = fopen("invertedIndex.txt", "w");
	createInvertIndex(t, fp);
	fclose(fp);
}

// print the tree to invertedIndex.txt in inflix order.
void createInvertIndex(Tree t, FILE *fp){
	if (t == NULL) return;
	createInvertIndex(left(t), fp);
	fprintf(fp, "%s", data(t));
	Link curr = t->urls->elems;
	while (curr != NULL) {
	        fprintf(fp, " ");
		fprintf(fp, "%s", curr->val);
		curr = curr->next;
	}
	fprintf(fp, "\n");
	createInvertIndex(right(t), fp);
}

// same as createInvertIndex but outputs to stdout for debugging
void showTree(Tree t){
	if (t == NULL) return;
	showTree(left(t));
	printf("%s", data(t));
	Link curr = t->urls->elems;
	while (curr != NULL) {
	        printf(" ");
		printf("%s", curr->val);
		curr = curr->next;
	}
	printf("\n");
	showTree(right(t));
}



