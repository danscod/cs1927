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

		//Add all children of temp to the frontier queue
		int i = 0;
		for (i = 0; i < g->nV; i++) {
			if (g->edges[temp][i] > 0 && g->edges[temp][i] <= max) {
				QueueJoin(frontier, i);
				//printf("i is: %d; g->edges[temp][i] is: %d\n", i, g->edges[temp][i]);
			}
		}
		

		//Add temp to the explored array
		explored[j] = temp;
		if (i > 1) {
			j++;
		}
		printf("j is: %d\n", j);
		printArray(explored, j);
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

/*
//this is my effort, tried to read ^^ but the recurrsion got weird, i think thats a depth first thing
//either way, theres a small bug somewhere if you need to go more than two nodes for some reason, probably inside the if
//ill keep working for now and commit anything new if its good and can do some more on sunday night if need be
//sorry if you dont like it
// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath(Graph g, Vertex src, Vertex dest, int max, int *path)
{
    //seems to have a problem if theres more than two cities
    printf("enter function\n");
    //check if valid
	assert(g != NULL && validV(g,src) && validV(g,dest));
	//make queue and path array
	//path = malloc(g->nV * sizeof(Vertex)); //already done
	Queue q = newQueue();
    //set up random valiables
	int found = 0;
	Vertex visited[g->nV];
	int i, j;
	Vertex tempPath[g->nV];
	//set all values to -1 so can check if has been visited
	for (i = 0; i < g->nV; i++) {
	   visited[i] = -1;
	}
	//set up initial queue
	Vertex temp = src;
	QueueJoin(q, temp);
	//go though queue V times
	while (found == 0){
	    //go until dest is found or return
        for (i = 0; i < g->nV; i++) {
            if (g->edges[temp][i] > 0 && g->edges[temp][i] <= max && visited[i] < 0) {
                //mark visited when put onto the queue so theres no repeats
                //put the value of the vertex it came from into the index of the vertex so it can be backtracked
                visited[i] = temp;
                if (i == dest){
                    //exit loop if destination is found
                    found = 1;
                    break;
                }
                QueueJoin(q, i);
            }
        }
        if (QueueIsEmpty(q)){
           //if the queue is empty then there is no path
           return -1;
        }
        //increment temp the be the next one on the queue
        temp = QueueLeave(q);
    }
    //by the time we get to here we should have found the destination (or returned) and have an array full of where vertices came from
    j = 0;
    //go through the visited and track where it came from
    while (i > 0){
        tempPath[j] = i;
        i = visited[i];
        j++;
    }
    j--;
    int size = j;
    //reverse array
    for (i = 0 ; j > -1; j--){
       path[i] = tempPath[j];
       i++;
    }
	return size + 1;
}

/*
z5015215@fife06:~/cs1927/labs/lab07$ ./travel Berlin Chicago
enter function
Least-hops route:
Berlin
->Chicago
z5015215@fife06:~/cs1927/labs/lab07$ ./travel Guam Manila
enter function
Least-hops route:
Guam
->Manila
z5015215@fife06:~/cs1927/labs/lab07$ ./travel Guam Bombay
enter function
Least-hops route:
Guam
->Bombay
z5015215@fife06:~/cs1927/labs/lab07$ ./travel Guam Baghdad
enter function
Least-hops route:
Guam
->Bombay
->Baghdad
z5015215@fife06:~/cs1927/labs/lab07$ ./travel Guam Berlin
enter function
Least-hops route:
Guam
->Bombay
->Berlin
z5015215@fife06:~/cs1927/labs/lab07$ ./travel Guam Montreal
enter function
Segmentation fault
z5015215@fife06:~/cs1927/labs/lab07$ ./travel Guam Berlin 2000
enter function
No route from Guam to Berlin
z5015215@fife06:~/cs1927/labs/lab07$ ./travel Guam Montreal 20000
enter function
Least-hops route:
Guam
->Montreal
*/
*/
