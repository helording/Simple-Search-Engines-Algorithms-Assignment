// Illustration of data

//  ------ url.txt ------ 
/*
#start Section-1

    url2  url34  url1 url26
    url52 url21
    url74  url6 url82

    #end Section-1

    #start Section-2

    Mars has long been the subject of human interest. Early telescopic observations
    revealed color changes on the surface that were attributed to seasonal vegetation
    and apparent linear features were ascribed to intelligent design.

    #end Section-2
*/
    
// ------ collection.txt ------ 
/*
url25   url31 url2
       url102   url78
    url32  url98 url33
*/

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

/*
Modifed from exercises in week 3 sorter.c
// sorter.c ... sorting algorithm analysis
// Written by John Shepherd, April 2013



*/
// reads collection.txt and scans for urls adding each to a linked list

int ncompares = 0;
int nswaps = 0;

FILE *openFile(char *name){
	// returns file pointer
	
	// add .txt to file name
    char* extension = ".txt";
    char fileName[strlen(name)+strlen(extension)+1];
	snprintf(fileName, sizeof(fileName), "%s%s", name, extension);
	
	// open the file
	FILE *fp = fopen(fileName, "r");

	// make sure file is open
	if (fp == NULL) {
		fprintf(stderr, "File %s cannot be found. Exiting.\n", fileName);
		exit(0);
	}
	//printf("%p\n", fp);
	// return file pointer to use later
	return fp;
}

// from a collection of urls, returns a list of them
Set GetCollection(FILE *fp){

	// create new list
	char url[10];
	Set urlSet = newSet();
	
	// while not at the end of the collection, add url to list
	while (fscanf(fp, "%s", url) == 1){
		insertInto(urlSet, url);
	}
	
	return urlSet;
}

// returns a graph, including adjacency matrix, of a collection of url
Graph makeGraph(Set urls){
	// initialise graph
	Graph g = newGraph(urls->nelems);
	// pointer to start of url list
	Link currURL = urls->elems;
	
	// add url as vertices in order
	while (currURL != NULL){
		addVertex(g, currURL->val);
		currURL = currURL->next;
	}
	
	//reset pointer
	currURL = urls->elems;
	
	// while not at the end of the list, open file and find strings. If string is a url, add edge between url opened and url found.
	while (currURL != NULL){
		FILE *urltxt = openFile(currURL->val);
		
		char url[10];
		
		while (fscanf(urltxt, "%s", url) == 1){
			// break if section 2 is reached
			if (strcmp(url, "Section-2") == 0) {
				break;
			// skip if string is not a url or if string is the same as investigated url
			} else if (strstr(url, "url") == NULL || strcmp(currURL->val, url) == 0){
				continue;
			// otherwise add url to the graph
			} else {
				addEdge(g, currURL->val, url);
			}
			
		}
		
		// close url file
		if ((fclose(urltxt)) != 0){
			fprintf(stderr, "error, file not closed, exiting\n");
			exit(0);
		}
		currURL = currURL->next;
	}
	
	return g;
}

intA newIntA(Graph g) {
	intA i = malloc(sizeof(struct intListRep) + 1);
	assert(i != NULL);
	i->ints = calloc(g->nV + 1, 4);
	assert(i->ints != NULL);
	
	int j = 0;
	for (; j <= g->nV; j++) i->ints[j] = -1;
	
	i->count = 0;
	
	return i;
}


// returns array of index numbers, each of which are a referrer of the url.
intA referrers(Graph g, char *url) {
	int i;
	intA r = newIntA(g);
	int id = vertexID(url, g->vertex, g->nV);
	
	for (i = 0; i < g->nV; i++){
		if (g->edges[i][id] == 1){
			r->ints[r->count] = i;
			r->count++;
		}
	}
	
	// if theres not referrers return -1;
	if (r->count == 0) {
		free(r);
		return NULL;
	}
	
	return r;
}

// returns an array of index numbers of a matrix, each of which are a referrence of the url.
intA references(Graph g, char *url) {
	int i;
	intA r = newIntA(g);
	int id = vertexID(url, g->vertex, g->nV);
	
	for (i = 0; i < g->nV; i++){
		if (g->edges[id][i] == 1){
			r->ints[r->count] = i;
			r->count++;
		}
	}
	
	// if theres not referrers return -1;
	if (r->count == 0) {
		free(r);
		return NULL;
	}
	
	return r;
}

// same as the function the a similar name but instead of char input index number input 
intA referrersINT(Graph g, int j) {
	int i;
	intA r = newIntA(g);
	
	for (i = 0; i < g->nV; i++){
		if (g->edges[i][j] == 1){
			r->ints[r->count] = i;
			r->count++;
		}
	}
	
	// if theres not referrers return -1;
	/*if (r->count == 0) {
		free(r);
		return NULL;
	}*/
	
	return r;
}

// same as the function the a similar name but instead of char input index number input 
intA referencesINT(Graph g, int j) {
	int i;
	intA r = newIntA(g);
	
	for (i = 0; i < g->nV; i++){
		if (g->edges[j][i] == 1){
			r->ints[r->count] = i;
			r->count++;
		}
	}
	
	// if theres not referrers return -1;
	/*if (r->count == 0) {
		free(r);
		return NULL;
	}*/
	
	return r;
}

// counts the In links of a url
int countIn(Graph g, int r){
	if (r == -1) return 0;
	int count = 0;
	int i = 0;
	for (i = 0; i < g->nV; i++){
		if (g->edges[i][r] == 1){			
			count++;
		}
	}
	return count;
}

// counts the out links of a url (returns raw number);
int countOut2(Graph g, int r){
	if (r == -1) return 0;
	int count = 0;
	int i = 0;
	for (i = 0; i < g->nV; i++){
		if (g->edges[r][i] == 1){
			count++;
		}
	}
	return count;
}

// counts the out links of a url (if 0 outlinks returns 0.5)
double countOut(Graph g, int r){
	if (r == -1) return 0;
	int count = 0;
	int i = 0;
	for (i = 0; i < g->nV; i++){
		if (g->edges[r][i] == 1){
			count++;
		}
	}
	if (count == 0){
		return 0.5;
	} else {
		return count;
	}
}



/*
int countIntArray(Graph g, int *list){
	int i = 0;
	while (list[i] != -1 || i < g->nV) i++;
	return i;
}*/

// two more functions that can find referrences and referrees from integers OR you modify the graph function....


// calculate Wout from a url and a given referrer 
double calcWin(Graph g, int pj, int pi) {
	int i = 0;
	double denominator = 0;
	double numerator = 0;

	intA references = referencesINT(g, pj);
	
	for (i = 0; i < g->nV; i++) {
		// if reach the end of the references break
		if (references->ints[i] == -1) break;

		// add the next reference I onto denominator	
		denominator = countIn(g, references->ints[i]) + denominator;
	}

	numerator = countIn(g, pi);
	free(references->ints);
	free(references);
	return numerator/denominator;
}

// calculate Win from a url and a given referrer 
double calcWout(Graph g, int pj, int pi) {
	int i = 0;
	double denominator = 0;
	double numerator = 0;
	
	intA references = referencesINT(g, pj);

	for (i = 0; i < g->nV; i++) {
		// if reach the end of the references break		
		if (references->ints[i] == -1) break;	
		denominator = countOut(g, references->ints[i]) + denominator;
	}
	
	numerator = countOut(g, pi);
	//printf("numerator is %lf, denominator is %lf\n", numerator, denominator);
	free(references->ints);
	free(references);
	return numerator/denominator;
}

// calculate PR(pj,pi)*wIn(pj,pi)*wOut(pj,pi) for a referrer pj and a url pi
double calcSumMultiple(Graph g, double PR, int pj, int pi){
	if (pj == -1 || pj >= g->nV) {
		fprintf(stderr, "Invalid reference or url. Returning -1.");
		return -1;
	}

	double wIn = calcWin(g, pj, pi);
	double wOut = calcWout(g, pj, pi);

	return PR*wIn*wOut;
}

// calculate total page rank of a url
double calcPR(Graph g, double d, double N, int pi, double *PR){
	int i = 0;	
	double sum = 0;

	intA refs = referrersINT(g, pi);	

	for (i = 0; i < g->nV; i++){
		// if references finish, break
		if (refs->ints[i] == -1) {
			break;
		} else {

			sum = calcSumMultiple(g, PR[refs->ints[i]], refs->ints[i], pi) + sum;
			//sum += PR[refs->ints[i]] / countOut(g, refs->ints[i]);
			//printf("sum is now %lf\n", sum);
		} 
	}
	free(refs->ints);
	free(refs);
	return (1-d)/N + (d*sum);
	
}

//normalise a word
char *normalise(char word[]){
    
    int l = strlen(word);
    char *nword = calloc(1,l+2);
    assert(nword != NULL);
    char *nnword = calloc(1,l+2);
    assert(nnword != NULL);
    int i = 0;
    int j = 0;
    for (i = 0; i < l; i++){
        nword[i] = tolower(word[i]); 
    }
    
    for (i = 0; i < l; i++){
        if ((nword[i] <= 'z' && nword[i] >= 'a') || (nword[i] <= '9' && nword[i] >= '0') ){
            nnword[j] = nword[i];
            j++;
        }
    }
    free(nword);
    return nnword; 
}


// Functions for bubble sort, used to sort a the page ranks of urls. Modifed from exercises in week 3 sorter.c
// sorter.c ... sorting algorithm analysis
// Written by John Shepherd, April 2013

int less(double x, double y)
{
	ncompares++;
	return (x < y);
}


void dswap(double a[], int i, int j)
{
	nswaps++;
	double tmp = a[i]; a[i] = a[j]; a[j] = tmp;
}

void iswap(int a[], int i, int j)
{
	int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
}


void bubbleSort(double a[], int lo, int hi, int b[])
{
   int i, j, nswaps;
   for (i = lo; i < hi; i++) {
      nswaps = 0;
      for (j = hi; j > i; j--) {
         if (!less(a[j], a[j-1])) {
            dswap(a,j,j-1);
            iswap(b,j,j-1);
            nswaps++;
         }
      }
      //monitor("Next");
      if (nswaps == 0) break;
   }
}


void ibubbleSort(int a[], int lo, int hi)
{
   int i, j, nswaps;
   for (i = lo; i < hi; i++) {
      nswaps = 0;
      for (j = hi; j > i; j--) {
         if (iless(a[j], a[j-1])) {
            iswap(a,j,j-1);
            nswaps++;
         }
      }
      //monitor("Next");
      if (nswaps == 0) break;
   }
}



// Functions to sort a FreqRank array, an array of structs which include any type of rank of a url and the word frequency given arguments. Modifed from exercises in week 3 sorter.c
// sorter.c ... sorting algorithm analysis
// Written by John Shepherd, April 2013
int iless(int x, int y)
{
	return (x < y);
}


int dless(double x, double y)
{

	return (x < y);
}


void fswap(FA a[], int i, int j)
{
	FA tmp = a[i]; a[i] = a[j]; a[j] = tmp;
}


void sortFA(FA fa[], int lo, int hi){
  int i, j, nswaps;
   for (i = lo; i < hi; i++) {
      nswaps = 0;
      for (j = hi; j > i; j--) {
         if (!iless(fa[j]->freq, fa[j-1]->freq)) {
            fswap(fa,j,j-1);
            nswaps++;
         }
      }
      //monitor("Next");
      if (nswaps == 0) break;
   }

   for (i = lo; i < hi; i++) {
      nswaps = 0;
      for (j = hi; j > i; j--) {
         if ((!dless(fa[j]->rank, fa[j-1]->rank)) && (fa[j]->freq == fa[j-1]->freq)) {
            fswap(fa,j,j-1);
            nswaps++;
         }
      }
      //monitor("Next");
      if (nswaps == 0) break;
   }	
	

}



/* MAIN TESTING
int main(int argc, char *argv[]){

	// IMPORTANT
	// compile with gcc readData.c set.c graph.c -o readData
	// run with ./readData Collection
	
	if (argc != 2) {
		fprintf(stderr, "incorrect number of arguments\n");
		exit(0);
	}
	
	char *name = argv[1];
	
	FILE *fp = openFile(name);
	
	Set urls = GetCollection(fp);
	showSet(urls);
	
	if ((fclose(fp)) != 0){
		fprintf(stderr, "error, file not closed, exiting\n");
		exit(0);
	}
	
	Graph g = makeGraph(urls);
	
	showGraph(g,1);
	
	int i = 0;

	printf("g->nV is %d\n", g->nV);
	//intA r1 = referrers(g, "url1");
//	intA r2 = references(g, "url1");
	intA r3 = referrersINT(g, 0);
	intA r4 = referencesINT(g, 0); 
	printf("Referrers of url1 are:\n");
	for (i = 0; i < g->nV; i++) printf("r[%d] is %d\n", i, r3->ints[i]);
	printf("url1 has %d referrers\n\n", r3->count);
	printf("References of url1 are:\n");
	for (i = 0; i < g->nV; i++) printf("r[%d] is %d\n", i, r4->ints[i]);
	printf("url1 has %d references\n", r4->count);*/

/*
	double winIN = 0;

	
	winIN = calcWin(g, 0, 2);

	printf("calcWin(0, 2) is %lf\n", winIN);
	
	double wOut = calcWout(g, 0, 2);

	printf("Wout(0 , 2) is %lf\n", wOut);


	printf("\n\n trying calc sum multiple\n");

	double N = urls->nelems;
	//diff = diffPR;
	
	double PR[g->nV];
	double PR2[g->nV];
	
	// make all PR equal 1/N;
	for (i = 0; i < g->nV; i++) PR[i] = 1/N;

	double total = 0;
	int test = 0;
	for (test = 0; test < g->nV; test++) {
		printf("\ncalc page rank of %d\n", test);
		PR2[test] = calcPR(g, 0.85, g->nV, test, PR) + total;
		 printf("page rank %d is %lf\n\n",test,PR2[test]);
		
	}
	
	//for (i = 0; i < g->nV; i++) printf("page rank %d is %lf\n",i,PR2[i]);
	return 0;
}*/



// TO DO WHEN YOU GET BACK.... ACUTALLY MAP OUT YOUR SHITTY WEB AND TEST IT 

// SEG FAULTING FROM referencesINT returning NULL (when count ==0)ssss







