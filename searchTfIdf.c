// libraries 
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


#define MAX_L 2000 

int main(int argc, char *argv[]){
	
	// variables
	int i = 1;
	int j = 0;
	double N;
	Link curr;

	// malloc a array of words and move the arguments to this array. Used to make loops more convenient 
	char **words = malloc(MAX_L);
	assert(words != NULL);
	for (; i < argc; i++){
		words[j] = argv[i];
		j++;
	}


	// produce a list of urls that contain argument words
	Set urls = wordURLS(argc, argv);


	// create a set of urls from collection.txt. Used to find N for idf rank variable.
	FILE *fp = openFile("collection");
	Set findUrlNum = GetCollection(fp);
	N = findUrlNum->nelems;
	disposeSet(findUrlNum);
	fclose(fp);
		
	

	// For each url assign a FA struct, containing the name of the url, its rank and word frequency. Each url and its values are placed in an array called frArray
	curr = urls->elems;
	FA frArray[urls->nelems];
	i = 0;
	j = 0;
	while (curr != NULL) {
		FA fa = malloc(sizeof(struct FreqRank) + 100);
		assert(fa != NULL);
		
		fa->name = calloc(1 ,strlen(curr->val) + 100);
		memcpy(fa->name, curr->val, strlen(curr->val) + 1);

		// Sum of all ITF-IDF ranks for each argument word for given url
		for (i = 0; i < argc - 1; i++){
			fa->rank = calcTF(words[i], curr->val)*calcIDF(words[i], N) + fa->rank;
		}

		fa->freq = checkFreq(curr->val, words, argc -1);
		
		frArray[j] = fa;
		j++;
		curr = curr->next;

	}

	// Sort the list of FA structs	
	sortFA(frArray, 0, urls->nelems - 1);
	
	// output the array or the first 30 elements
	for (i = 0; i < urls->nelems; i++) {
		fprintf(stdout, "%s %.6f\n", frArray[i]->name, frArray[i]->rank);
		if (i == 29) break;
	}

	// free array
	for (i = 0; i < urls->nelems; i++) {
		free(frArray[i]->name);
		free(frArray[i]);
		
	}
	
	// free words and urls set
	disposeSet(urls);
	free(words);

	return 0;



}

// calculates the idf value for a word in a collection
double calcIDF(char *word, double numDocs){
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

	if (count > 0){
		double IDF = log10(numDocs/count);
		return IDF;
	} else {
		return 0;
	}
}

// find the tf value for a word in a url
double calcTF(char *word, char *document){
	
	FILE *fp = openFile(document);

	// varibles
	int start_reading = 0;
	double count = 0;
	double wordCount = 0;
	char scanned[MAX_L];
	char *nScanned;

	// scan through section two and find matches
	while ((fscanf(fp, "%s", scanned)) != EOF){
		if (strcmp(scanned, "Section-2") != 0 && start_reading != 1){
			continue;
		} else if (strcmp(scanned, "Section-2") == 0) {
			start_reading = 1;
			continue;
		}

		if (strcmp(scanned, "#end") == 0){
			break;
		}		

		nScanned = normalise(scanned);
		if (strcmp(nScanned, word) == 0){
			count++;
		}
		free(nScanned);
	}

	fseek(fp, 0L, SEEK_SET);
	start_reading = 0;	

	// find total words
	while ((fscanf(fp, "%s", scanned)) != EOF){
		if (strcmp(scanned, "Section-2") != 0 && start_reading != 1){
			continue;
		} else if (strcmp(scanned, "Section-2") == 0) {
			start_reading = 1;
			continue;
		}

		if (strcmp(scanned, "#end") == 0){
			break;
		}		
		wordCount++;

	}
	if (wordCount > 0){
		return count/wordCount;
	} else {
		return 0;
	}
	
}

// returns a list of urls that have the word. Same function used in searchPagerank.c
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

// find the argument word frequency of a url. Same function used in searchPagerank.c
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

