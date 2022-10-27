#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "set.h"
#include "graph.h"
#include "readData.h"
#include "BSTree.h"
#include "searchPagerank.h"

#define MAX_L 2000

int main(int argc, char *argv[]){

	// variables
	int i = 1;
	int j = 0;
	Link curr;

	// malloc a array of words and move the arguments to this array. Used to make loops more convenient 
	char **words = malloc(MAX_L);
	assert(words != NULL);
	for (; i < argc; i++){
		words[j] = argv[i];
		j++;
	}

	//for (i = 0; i < argc -1; i++) printf("%s\n", words[i]);

	// produce a list of urls that contain argument words
	Set urls = wordURLS(argc, argv);

	//showSet(urls);
	//int urlFreq[argc - 1];

	// For each url assign a FA struct, containing the name of the url, its rank and word frequency. Each url and its values are placed in an array called frArray
	curr = urls->elems;
	FA frArray[urls->nelems];
	i = 0;
	while (curr != NULL) {
		FA fa = malloc(sizeof(struct FreqRank) + 100);
		assert(fa != NULL);
		
		fa->name = calloc(1 ,strlen(curr->val) + 100);
		memcpy(fa->name, curr->val, strlen(curr->val) + 1);

		fa->rank = searchUrlRank(curr->val);
		fa->freq = checkFreq(curr->val, words, argc -1);
		
		//printf("%s %d %lf\n", fa->name, fa->freq, fa->rank);
		frArray[i] = fa;

		i++;
		curr = curr->next;
	}

	
	/*
	for (i = 0; i < urls->nelems; i++) {
		printf("%s %d %lf\n", frArray[i]->name, frArray[i]->freq, frArray[i]->rank);
	}*/

	// Sort the list of FA structs
	sortFA(frArray, 0, urls->nelems - 1);

	/*
	for (i = 0; i < urls->nelems; i++) {
		printf("%s %d %lf\n", frArray[i]->name, frArray[i]->freq, frArray[i]->rank);
	}*/

	// output the array or the first 30 elements
	for (i = 0; i < urls->nelems; i++) {
		fprintf(stdout, "%s\n", frArray[i]->name);
		if (i == 29) break;
	}

	// free array
	for (i = 0; i < urls->nelems; i++) {
		free(frArray[i]->name);
		free(frArray[i]);
		
	}
	// free the words and urls set
	disposeSet(urls);
	free(words);
	return 0;
}



// returns a list of urls that have the word
Set wordURLS(int argc, char *argv[]){
	FILE *fp = openFile("invertedIndex");

	int success = 0;
	Set urls = newSet();
	int i = 1;
	char scanned[MAX_L];

	while (i < argc) {
		fseek(fp, 0L, SEEK_SET);

		while ((fscanf(fp, "%s", scanned)) != EOF){

			if (strcmp(scanned, argv[i]) == 0){
				success = fscanf(fp, "%s", scanned);
				while (strstr(scanned, "url") != NULL && success == 1){
					insertInto(urls, scanned);
					success = fscanf(fp, "%s", scanned);
				}
				break;
			}
		}

		i++;
	}
	return urls;

}

// find the corresponding rank of the url
double searchUrlRank(char *url){

	FILE *fp = openFile("pagerankList");

	char scanned[MAX_L];
	double rank;
 	char *nScanned;
	
	while ((fscanf(fp, "%s", scanned)) != EOF){

		nScanned = normalise(scanned);
		if (strcmp(nScanned, url) == 0){
			fscanf(fp, "%s", scanned);
			fscanf(fp, "%lf", &rank);
			//rank = atof(scanned);
			return rank;
			}
		free(nScanned);
		}

	return 0;
}

// find the argument word frequency of a url
int checkFreq(char *url, char **word, int stringS){
	int i = 0;
	int count = 0;
	int success = 0;
	FILE *fp = openFile("invertedIndex");

	char scanned[MAX_L];
	
	while (i < stringS) {
		fseek(fp, 0L, SEEK_SET);

		while ((fscanf(fp, "%s", scanned)) != EOF){

			if (strcmp(scanned, word[i]) == 0){
				success = fscanf(fp, "%s", scanned);
				while (strstr(scanned, "url") != NULL && success == 1){
					if ((strcmp(scanned, url)) == 0){
						count++;
						break;
					}
					success = fscanf(fp, "%s", scanned);
				}
				break;
			}
		}

		i++;
	}
	return count;
}
/*
int success = 0;
	double count = 0;
	char scanned[MAX_L];
	FILE *fp = openFile("invertedIndex");

	while ((fscanf(fp, "%s", scanned)) != EOF){
		if (strcmp(scanned, word) == 0){
			success = fscanf(fp, "%s", scanned);
			while (strstr(scanned, "url") != NULL && success == 1){
				count++;
				success = fscanf(fp, "%s", scanned);
			}
			break;
		}

	}
*/
