// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
#include "Queue.h"

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int    nV;    // #vertices
	int    nE;    // #edges
	int  **edges; // matrix of weights (0 == no edge)
} GraphRep;

// check validity of Vertex
int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	Edge new = {v,w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge(Graph g, Vertex v, Vertex w, int wt)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] == 0) {
		g->edges[v][w] = wt;
		g->edges[w][v] = wt;
		g->nE++;
	}
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] != 0) {
		g->edges[v][w] = 0;
		g->edges[w][v] = 0;
		g->nE--;
	}
}

// create an empty graph
Graph newGraph(int nV)
{
	//Initialise
	assert(nV > 0);

	//Create the graph
	Graph g = malloc(sizeof(GraphRep));
	assert(g != NULL);

	//Create the adjacency matrix
	int **e = malloc(nV * sizeof(int *));
	int i = 0;
	for (i = 0; i < nV; i++) {
		e[i] = calloc(nV, sizeof(int));
		assert(e[i] != NULL);
	}
	g->edges = e;
	assert(g->edges != NULL);

	//Set no. of vertices to match input
	g->nV = nV;
	g->nE = 0;		//There are no edges yet...

	return g;
}

// free memory associated with graph
void dropGraph(Graph g)
{
	assert(g != NULL);
	// not needed for this lab
}

// display graph, using names for vertices
void showGraph(Graph g, char **names)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n",g->nV,g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d %s\n",v,names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf("\t%s (%d)\n",names[w],g->edges[v][w]);
			}
		}
		printf("\n");
	}
}


int exploreGraph(Graph g, Vertex dest, int max, int *explored, Queue frontier, int j);
void printArray(int *explored, int j);
// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath(Graph g, Vertex src, Vertex dest, int max, int *explored)
{
	//Setup
	assert(g != NULL && validV(g,src) && validV(g,dest));

	Queue frontier = newQueue();
	QueueJoin(frontier, src);

	int j = 0;

	j = exploreGraph(g, dest, max, explored, frontier, j);

	printf("exploreGraph has been exited\n");
	printArray(explored, j);
	printf("EXITING findPath. j is : %d\n", j);


	return j;
}

int exploreGraph(Graph g, Vertex dest, int max, int *explored, Queue frontier, int j) {
	Vertex temp = QueueLeave(frontier);
	printf("Vertex temp is: %d\n", temp);
	if (temp == dest) {
		return j; 
	} else {
		//Add temp to the explored array
		explored[j] = temp;
		j++;
		printf("j is: %d\n", j);
		printArray(explored, j);

		//Add all children of temp to the frontier queue
		int i = 0;
		for (i = 0; i < g->nV; i++) {
			if (g->edges[temp][i] > 0 && g->edges[temp][i] <= max) {
				QueueJoin(frontier, i);
				//printf("i is: %d; g->edges[temp][i] is: %d\n", i, g->edges[temp][i]);
			}
		}
		exploreGraph(g, dest, max, explored, frontier, j);
	}
	printf("I shouldn't be here\n");
	return j;
}


void printArray(int *explored, int j) {
	printf("This is what int *explored looks like:\n");
	int arrayCounter = 0;
	for (arrayCounter = 0; arrayCounter < 30; arrayCounter++) {
		printf("%d, ", explored[arrayCounter]);
	}
	printf("\nAbout to leave...\n");	
}
/*
	for (j = 0; j < g->nV; j++) {
		src = QueueLeave(q);
		for (i = 0; i < g->nV; i++){
			if (g->edges[src][i] > 0 && visited[i] == 0 && g->edges[src][i] <= max){
				QueueJoin(q, i);
				visited[i] = 1;
			}
			if (i == dest){
				return j;
			}
		}
	}
*/