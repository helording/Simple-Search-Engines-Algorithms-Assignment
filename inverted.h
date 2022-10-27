#ifndef INVERTED_H
#define INVERTED_H


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "set.h"
#include "graph.h"
#include "readData.h"
#include "BSTree.h"
#include <ctype.h> 

Tree createWordTree(Set urls);
void createII(Tree t);
void createInvertIndex(Tree t, FILE *fp);
int checkFreq(char *url, char **word, int stringS);

#endif
