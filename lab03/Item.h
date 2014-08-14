// Item.h - String Items for use in GADTs
// Written by John Shepherd, March 2013

#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <string.h>

typedef char *Item;

#define ItemCopy(s) strdup((s))
#define ItemEQ(s,t) (strcmp((s),(t)) == 0)
#define ItemLT(s,t) (strcmp((s),(t)) < 0)
#define ItemLE(s,t) (strcmp((s),(t)) <= 0)
#define ItemGT(s,t) (strcmp((s),(t)) > 0)
#define ItemGE(s,t) (strcmp((s),(t)) >= 0)
#define ItemScan(s) strdup((s))
#define ItemShow(f,s) fprintf((f),"%s",(s))

#endif
