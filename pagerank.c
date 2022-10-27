#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "graph.h"
#include "readData.h"
#include <math.h>

int main(int argc, char *argv[]){

	//  PageRankW(int d, int diffPR, int maxIterations)
	
	// check arguments
	if (argc != 4) {
		fprintf(stderr, "incorrect number of arguments\n");
		exit(0);
	}

	// variables
	int i = 0;
	int j = 0;
	double N;
	int iteration = 0;

	double d = atof(argv[1]);
	double diffPR = atof(argv[2]);
	int maxIterations = atoi(argv[3]);
	
	double diff = diffPR;

	// open colleciton and make a set of urls
	FILE *fp = openFile("collection");
	Set urls = GetCollection(fp);

	// close colleciton
	if (fclose(fp)!= 0){
		fprintf(stderr, "error, file not closed, exiting\n");
		exit(0);
	}
	
	// make an adacency matrix
	Graph g = makeGraph(urls);
	
	
	// set N to the number of urls in the collection, diff to diffPR
	N = urls->nelems;
	
	double PR[g->nV];
	double PR2[g->nV];
	

	// make all PR equal 1/N;
	for (i = 0; i < g->nV; i++) PR[i] = 1/N;
	
	while (iteration < maxIterations && diff >= diffPR) {
		diff = 0;
		if (iteration % 2 == 0) {
			// calc page rank
			for (i = 0; i < g->nV; i++) {
				PR2[i] = calcPR(g, d, N, i, PR);
			}
			// calc diff
			for (j = 0; j < g->nV; j++){
				diff = diff + fabs(PR2[j] - PR[j]);
			 }

		} else {
			// calc page rank
			for (i = 0; i < g->nV; i++) {
				PR[i] = calcPR(g, d, N, i, PR2);
			}
			// calc diff
			for (j = 0; j < g->nV; j++){
				diff = diff + fabs(PR[j] - PR2[j]);
			 }
		}

		iteration++;
	}

	// if last iteration change PR2, then swap PR equal PR2
	if (iteration % 2 != 0){
		for (i = 0; i < g->nV; i++) PR[i] = PR2[i];
	}
	
	// create a array of urls so we can track which page rank corresponds to which url when we sort the list
	int urlTrack[g->nV];
	for (i = 0; i < g->nV; i++) urlTrack[i] = i;

	// sort the page ranks 
	bubbleSort(PR, 0, g->nV, urlTrack);

	// open pagerankList for writing
	FILE *ip = fopen("pagerankList.txt", "w");

	// if pagerankList.txt doesnt open exit
	if (ip == NULL) {
		fprintf(stderr, "Couldn't open file exiting...\n");
		exit(0);
	}
	
	// print to pagerankeList.txt 
	for (i = 0; i < g->nV; i++) {
		Link currURL = urls->elems;
		for (j = 0; j < urlTrack[i] ; j++) currURL = currURL->next;
		fprintf(ip, "%s, ", currURL->val);
		fprintf(ip, "%d, ", countOut2(g, urlTrack[i]));
		fprintf(ip, "%.7f\n", PR[i]);

	}
	// close and free variables
	fclose(ip);
	disposeGraph(g);
	disposeSet(urls);

	return 0;
}



