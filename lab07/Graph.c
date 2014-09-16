// OpenLearning Group: "Danapalooza"
// Wednesday 18 Drum
// Danilo Scodellaro & Daniel Lohrey
// z3414551 | z5015215

// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
#include "Queue.h"

#define TRUE 1
#define FALSE 0

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

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
// CURRENT ISSUE: SHORTEST PATHS BETWEEN TWO CITIES SEEM TOO LONG. POSSIBLE
// ERROR IN COMPARISON BETWEEN MAX AND ADJACENCY MATRIX VALUES
int findPath(Graph g, Vertex src, Vertex dest, int max, int* bestPath) {

	//Setup
	//Ensure valid inputs
	assert(g != NULL);
	assert(validV(g,src));
	assert(validV(g,dest));	

	//Create necessary variables
	int *explored = calloc(g->nV, sizeof(int));		//Array of visited nodes (all set to 0)
	Vertex *path = calloc(g->nV, sizeof(Vertex));	//Array of nodes within the path
	Queue q = newQueue();							//Queue of frontier nodes
	QueueJoin(q, src);								//Add the source Vertex to the Queue (first node to be explored)
	int isLocated = FALSE;							//Boolean flag to see if destination has been located (#defined)

	//Fill the given array (bestPath) with -1s (allows us to calculate length later)
	int i;
	for (i = 0; i < g->nV; i++) {
		bestPath[i] = -1;
	}

	//Traverse the graph
	while (QueueIsEmpty(q) == FALSE && isLocated == FALSE) {
		//printf("While loop entered\n");
		Vertex y = QueueLeave(q);					//Dequeue the first vertex in the queue,
		Vertex x = y;								//and store as temporary vertices x and y
		if (explored[x] == TRUE) {					//If we've already explored this node, then move on 
			continue;
		}
		explored[x] = TRUE;							//Mark this node as explored

		//Loop within the level to determine path
		for (y = 0; y < g->nV; y++) {
			//Abort loop is the vertex has already been visited, or edge is nonexistent/too long
			if (explored[y] == TRUE || g->edges[x][y] == FALSE 
				|| g->edges[x][y] > max) {
				continue;
			}
			//Record node in path array (allows us to deconstruct into bestPath)
			path[y] = x;
			//Abort loop if the node y is dest
			if (y == dest) {
				isLocated = TRUE;
				break;
			}
			//If not has not been explored, add to queue
			if (explored[y] == FALSE) {
				QueueJoin(q,y);
			}
		}
	}

	//Generate bestPath
	//PROBLEM - PATH IS GENERATED IN REVERSE ORDER. MUST FIX
	if (isLocated == FALSE) {
		return -1;
	} else {
		Vertex temp;
		for (temp = dest, i = 0; temp != src; temp = path[temp], i++) {
			bestPath[i] = temp;
		}
		bestPath[i] = src;
	}

	//Determine length of bestPath
	for (i = 0; i < g->nV; i++) {
		if (bestPath[i] == -1) {
			break;
		}
	}
	
	//reverse order of array
	int j;
	Vertex temp;
	for (j = 0; j < (i+1)/2; j++){
	   temp = bestPath[j];
	   bestPath[j] = bestPath[i - j - 1];
	   bestPath[i - j - 1] = temp;
	}

	//Return length of bestPath which is occupied by meaningful values
	return i;
}

// REFERENCES
// Based on the following code by John Shepherd, located iat:
// /~cs1927/14s2/lecs/week06mon/exercises/GraphLab 
/*
// iterative BFS algorithm to print path src...dest

void findPath(Graph g, Vertex src, Vertex dest)
{
   visited = calloc(g->nV,sizeof(int));
   Vertex *path = calloc(g->nV,sizeof(Vertex));
   Queue q = newQueue();
   QueueJoin(q,src);
   int isFound = 0;
   while (!QueueIsEmpty(q) && !isFound) {
      Vertex y, x = QueueLeave(q);
      if (visited[x]) continue;
      visited[x] = 1;
      printf("x=%d, q=",x);
      for (y = 0; y < g->nV; y++) {
         if (g->edges[x][y] == 0 || visited[y]) continue;
         path[y] = x;
         if (y == dest) { isFound = 1; break; }
         if (!visited[y]) QueueJoin(q,y);
      }
      showQueue(q);
   }
   if (!isFound)
      printf("No path from %d to %d\n",src,dest);
   else {
      // display path in dest..src order
      Vertex v;
      printf("Path: ");
      for (v = dest; v != src; v = path[v])
          printf("%d<-", v);
      printf("%d\n", src);
   }
}

*/
