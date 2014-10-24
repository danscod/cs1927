// words.c ... Build a table of words from a dictionary file
// Written by John Shepherd, August 2008
// Modified by John Shepherd, May 2013

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "Item.h"
#include "HashTable.h"

void usage(char *prog)
{
	fprintf(stderr, "Usage: %s FileName HashTabSize\n", prog);
	exit(1);
}

int main(int argc, char *argv[])
{
    char *fname;  // name of file containing words
    FILE *wordf;  // handle for dictionary file
	Item word;  // current word from file
    int size = 7919;  // default size of hash table
    HashTable htab;  // the hash table
	int nwords, nfound;  // counters

	// set up parameters
	switch (argc) {
	case 2: fname = argv[1]; break;
	case 3: fname = argv[1]; size = atoi(argv[2]); break;
    default: usage(argv[0]); break;
	}
	
    // access the word file
    wordf = fopen(fname,"r");
    if (wordf == NULL) {
        printf("Can't open %s\n",fname);
        exit(1);
    }
    printf("Reading words from %s\n",fname);    

	// build hash table, containing all words from file
	nwords = 0;
    htab = newHashTable(size);
	while ((word = ItemGet(wordf)) != NULL) {
		if (eq(word,"")) { dropItem(word); continue; }
		HashTableInsert(htab,word);
		nwords++;
	}

	// examine hash table
	HashTableStats(htab);

	// run some tests
	nfound = 0;
	rewind(wordf); srand(113);
	while ((word = ItemGet(wordf)) != NULL) {
		if (eq(word,"")) { dropItem(word); continue; }
		if (HashTableSearch(htab,key(word)) != NULL)
			nfound++;
		else
			fprintf(stderr,"Failed to find %s\n", word);
		dropItem(word);
	}
	assert(nfound == nwords);
    assert(HashTableSearch(htab,"aaaa") == NULL) ;
    assert(HashTableSearch(htab,"xxxx") == NULL) ;
    assert(HashTableSearch(htab,"yyyy") == NULL) ;
    assert(HashTableSearch(htab,"zzzz") == NULL) ;
	printf("Testing completed OK\n");

	// clean up
	fclose(wordf);
    dropHashTable(htab);
    return 0;
}
