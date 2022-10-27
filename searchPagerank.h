#ifndef SEARCHPAGERANK_H
#define SEARCHPAGERANK_H


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "set.h"
#include "graph.h"
#include "readData.h"
#include "BSTree.h"



Set wordURLS(int argc, char *argv[]);
int checkFreq(char *url, char **word, int stringS);
double searchUrlRank(char *url);

#endif
