//Danilo Scodellaro
//z3414551
//quiz2.c


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct GraphRep {
    int nV;  // vertex count
    int nE;  // edge count
    unsigned char **edges; // adjacency matrix
} *Graph;

Graph newGraph(int nV);

int main(int argc, char const *argv[]) {
    
    //Question 4
    printf("Question 4: sizeof datastruct\n");

    Graph test = newGraph(10);
    int size = sizeof(newGraph(10));

    printf("size is: %d", size);

    return EXIT_SUCCESS;
}

// create an empty graph
Graph newGraph(int nV)
{
    //Initialise
    assert(nV > 0);

    //Create the graph
    Graph g = malloc(sizeof(Graph));
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
    g->nE = 0;      //There are no edges yet...

    return g;
}