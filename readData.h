#ifndef READDATA_H
#define READDATA_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include "searchPagerank.h"
#include "set.h"
#include "graph.h"
#include "readData.h"
#include <math.h>
#include <ctype.h>

typedef struct FreqRank *FA;



typedef struct FreqRank {
	char *name;
	int freq;	
	double rank;
} FreqRank;


typedef struct intListRep *intA;

typedef struct intListRep {
  int *ints;
  int count;
} intListRep;

FILE *openFile(char *name);
Set GetCollection(FILE *fp);
Graph makeGraph(Set urls);
int addList(Set urls);
intA referrers(Graph g, char *url);
intA references(Graph g, char *url);
double calcWin(Graph g, int pj, int pi);
double calcWout(Graph g, int pj, int pi);
double countOut(Graph g, int r);
int countIn(Graph g, int r);
intA referencesINT(Graph g, int j);
intA referrersINT(Graph g, int j);
intA newIntA(Graph g);
double calcSumMultiple(Graph g, double PR, int pj, int pi);
char *normalise(char *word);
double calcPR(Graph g, double d, double N, int pi, double *PR);
double *bubSort(double array[]);
int less(double x, double y);
void dswap(double a[], int i, int j);
void iswap(int a[], int i, int j);
void bubbleSort(double a[], int lo, int hi, int b[]);
int countOut2(Graph g, int r);
int iless(int x, int y);
int dless(double x, double y);
void fswap(FA a[], int i, int j);
void sortFA(FA fa[], int lo, int hi);
void ibubbleSort(int a[], int lo, int hi);
#endif
