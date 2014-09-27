// Determine if/how two Places are adjacent

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include "Places.h"

int main(int argc, char **argv)
{
	int id1, id2, errs=0;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s Place1 Place2\n", argv[0]);
		exit(1);
	}

	// convert args to place IDs
	id1 = (strlen(argv[1]) == 2) ? abbrevToID(argv[1]) : nameToID(argv[1]);
	id2 = (strlen(argv[1]) == 2) ? abbrevToID(argv[2]) : nameToID(argv[2]);

	// check place validity
	if (id1 == NOWHERE) {
		errs++;
		fprintf(stderr, "Invalid place name: %s\n", argv[1]);
	}
	if (id2 == NOWHERE) {
		errs++;
		fprintf(stderr, "Invalid place name: %s\n", argv[2]);
	}
	if (errs > 0) exit(1);

	Map europe;
	europe = newMap();

	// find shortest path
	int i, n;
    Location path[NUM_PLACES];
	Transport trans[NUM_PLACES];

	printf("Starting from %s ...\n", idToName(id1));
	n = shortestPath(europe, id1, id2, path, trans);
	if (n == 0)
		printf("you cannot reach %s\n", idToName(id2));
	else {
		for (i = 1; i < n; i++) {
			if (i > 1 && n > 2) printf("then ");
			printf("go to %s by ", idToName(path[i]));
			switch (trans[i]) {
			case ROAD: printf("road\n"); break;
			case RAIL: printf("rail\n"); break;
			case BOAT: printf("boat\n"); break;
			default:   printf("????\n"); break;
			}
		}
		printf("You have reached your destination\n");
	}
	return 0;
}
