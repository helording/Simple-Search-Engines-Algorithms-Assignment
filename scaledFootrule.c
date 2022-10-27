#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include "readData.h"
#include "scaledFootrule.h"
#include "set.h"
#include "graph.h"

#define MAX_L 2000
#define ARRAY 500

// Global variables
double min = 200;
int modelP[200];


int main(int argc, char **argv) 
{
	// variables

	// number of files in argument line
	int fileNum = argc - 1;

	//urlNumArray holds the number of each url
	int urlNumArray[ARRAY][ARRAY];
	// a matrix of the index of each url in each ranking file
	double index[ARRAY][ARRAY];

	//int Bindex[ARRAY][ARRAY] = {0};
	//Number of urls in each file
	double urlCounter[ARRAY];	
	// list of all url numbers	
	int URLDataBase[ARRAY];
	// array with numbers counted from 1 .. number of urls
	double countedArray[ARRAY];

	double numUrls = 0;
	// if invalid number of arguments
	if(argc < 2) {
		fprintf(stderr, "Invalid number of arguments\n");
		exit(0);
	}


	// move arguments into files array
	int w = 1;
	int j = 0;
	char **files = calloc(MAX_L, 1);

	for (w = 1; w < argc; w++) {
		files[j] = argv[w];
		j++;
	}


	//storing number of url from each file into matrix 
	for(w = 0; w < fileNum; w++) 
	{
		FILE *fp;
		fp = fopen(files[w], "r");
		int x = 0;
		char string[MAX_L];
		int y;
		while(fscanf(fp, "%s", string) != EOF) 
		{
			char *urlNumChar = strtok(string, "url");
			int urlNumber = atoi(urlNumChar);
			urlNumArray[w][x] = urlNumber;
			
			// check if url is already in array
			y=0;
			while(y < numUrls) {
				if (URLDataBase[y] == urlNumber) 
				{
					break;
				}
				y++;
			}

			if (y == numUrls) {
				URLDataBase[(int)numUrls] = urlNumber;
				numUrls++;
			}

			x++;
			urlCounter[w]++;
		}
		fclose(fp);
	}
	
	w = 1;
	for (j = 0; j < numUrls; j++){
		countedArray[j] = w;
		w++;
	}

	// sorted list of all urls
	ibubbleSort(URLDataBase, 0, numUrls - 1);

	
	//indicies creates array with indexs of urls for each rank file given
	for (j = 0; j < fileNum; j++) 
	{
		indicies(index[j], urlNumArray[j], URLDataBase, numUrls);
	}

	//feed doComputation() function with values
	doComputation(index, countedArray, urlCounter, numUrls, fileNum, 0);

	int actModel[ARRAY];
	for (int w = 0; w < numUrls; w++) {
		for (int j = 0; j < numUrls; j++) {
			if (modelP[j] == w+1) {
				actModel[w] = j;	
				break;	
			}
			
		}
	}
	
	// print values to stdout
	printf("%lf\n", min);
	for(w = 0; w < numUrls; w++) 
	{
		printf("url%d\n", URLDataBase[actModel[w]]);

	}
	// free files
	free(files);

	return 0;

}



//finds order of indicies and determines minimum adjusted foot rule
void doComputation(double index[500][500], double *count, double *urlCounter, double numUrls,  int fileNum, int w)
{ 

	double tmp = 0;
	int x = 0;
	int y = 0;
	int z = 0;

  	while(z < fileNum) {
  		x = 0;
  		while(x < urlCounter[z]) {
  			tmp = fabs( (index[z][x] / urlCounter[z]) - (count[x] / numUrls) ) + tmp;
  			x++;
  		}
  		z++;
  	}

  	//if update modelp array to new best position
     	if (tmp < min) 
     	{
		//printf("");
	   	min = tmp;
     		for (y = 0; y < numUrls; y++) 
		{
	     		modelP[y] = count[y];     			
	     	}
     	}

  	//recurse through different counts
	for (x = w; x < numUrls; x++) 
	{ 
 		dswap(count,w,x);
		doComputation(index, count, urlCounter, numUrls, fileNum, w + 1);

	}
	return;
}


// find the ranking of the urls in each file
void indicies(double *indexArray, int *rankList, int *index, double maxL) 
{
	int indexNum = 0;
	int w = 0;
	int x = 0;

	for(w = 0; w < maxL; w++) 
	{
		for(x = 0; x < maxL; x++) 
		{
			if(index[w] == rankList[x])
			 {
				indexArray[indexNum] = x + 1;
				indexNum++;
				break;
			}
		}
	}
	return;
}
/*
void efficiency(double *indexArray[], int *rankList[], int *index, double urlCounter, double maxL, double numFile){
	int i = 0;
	int j = 0;
	int x = 0;
	// for each file 
	//for (j = 0; j < (int)numFile; j++){
		
		for (i = 0; i < maxL; i++){
			for(x = 0; x < maxL; x++) 
			{
				if(index[i] == rankList[x])
				 {
					indexArray[i] =  (urlCounter[i] - (x + 1)) + indexArray[i];
					indexNum++;
					break;
				}
			}
		}
	//}
	
}*/







