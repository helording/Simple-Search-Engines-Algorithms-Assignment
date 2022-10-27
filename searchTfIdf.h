#ifndef SEARCHITIDF_H
#define SEARCHITIDF_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "searchPagerank.h"
#include "searchTfIdf.h"
#include "set.h"
#include "graph.h"
#include "readData.h"

Set wordURLS(int argc, char *argv[]);
double calcIDF(char *word, double numDocs);
double calcTF(char *word, char *document);
int checkFreq(char *url, char **word, int stringS);
int docFreq(char *url, char **word, int stringS);


#endif
