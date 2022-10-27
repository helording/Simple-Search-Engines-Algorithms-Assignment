#ifndef SCALEDFOOTRULE_H
#define SCALEDFOOTRULE_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include "readData.h"
#include "set.h"
#include "graph.h"


void indicies(double *indexArray, int *rankList, int *index, double maxL);
void doComputation(double index[500][500], double *count, double *urlCounter, double numUrls,  int fileNum,int w);
//double doComp(int *array, int w, int range, int index[500][500], int *urlCounter, int fileNum);

#endif
