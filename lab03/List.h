// List.h - Interface to doubly-linked list GADT
// Written by John Shepherd, March 2013
// Last modified, August 2014

#ifndef DLIST_H
#define DLIST_H

#include <stdio.h>
#include "Item.h"

// External view of List
// Implementation given in intList.c

typedef struct ListRep *List;

// create a new empty List
List newList();

// free up all space associated with list
void freeList(List);

// create an List by reading items from a file
// assume that the file is open for reading
List getList(FILE *);

// display list on output, one item per line
void showList(FILE *, List);

// check sanity of a List (for testing)
int validList(List);

// return item at current position
Item ListCurrent(List);

// move current position (+ve forward, -ve backward)
// return 1 if reach end of list during move
int ListMove(List, int);

// move to specified position in list
// i'th node, assuming first node has i==1
int ListMoveTo(List, int);

// insert an item before current item
// new item becomes current item
void ListBefore(List, Item);

// insert an item after current item
// new item becomes current item
void ListAfter(List, Item);

// delete current item
// new item becomes item following current
// if current was last, current becomes new last
// if current was only item, current becomes null
void ListDelete(List);

// return number of elements in a list
int ListLength(List);

// is the list empty?
int ListIsEmpty(List);

#endif
