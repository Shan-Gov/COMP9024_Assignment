// Weighted Directed Graph ADT
// Adjacency Matrix Representation ... COMP9024 25T2
#include "WGraph.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


// A custom struct to hold all the ferry schedules
typedef struct FerryNode {
   int departTime;
   int arriveTime;
   struct FerryNode *next;
} FerryNode;


typedef struct EdgeInfo {
   bool isExist;
   bool isWalk;
   bool isFerry;
   int walkingTime;
   int departTime;
   int arriveTime;
   FerryNode *ferries; // a linked list of the ferry schedules. depart @ v & arrive @ w 
} EdgeInfo;


typedef struct GraphRep {
   EdgeInfo **edges;  // Adjusted to store struct EdgeInfo which contains all edge info.
   int nV;       // #vertices
   int nE;       // #edges
   char **names; // Names of the landmarks will go here 2D array
} GraphRep;


Graph newGraph(int V) {
   assert(V >= 0);
   int i;

   Graph g = malloc(sizeof(GraphRep));
   assert(g != NULL);
   g->nV = V;
   g->nE = 0;

   // allocate memory for each row
   g->edges = malloc(V * sizeof(EdgeInfo *));
   assert(g->edges != NULL);
   // allocate memory for each column and initialise with 0
   for (i = 0; i < V; i++) {
      g->edges[i] = malloc(V * sizeof(EdgeInfo));
      assert(g->edges[i] != NULL);

      for (int j = 0; j < V; j++) {
         g->edges[i][j].isExist = false;
         g->edges[i][j].isWalk = false;
         g->edges[i][j].isFerry = false;
         g->edges[i][j].walkingTime = NOT_WALKABLE;
         g->edges[i][j].departTime = NO_FERRY;
         g->edges[i][j].arriveTime = NO_FERRY;
         g->edges[i][j].ferries = NULL; // setting all ferry schedule pointers to NULL
      }
   }

   // Allocate the array for the names
   g->names = malloc(V * sizeof(char *));
   for (int j = 0; j < V; j++) {
      g->names[j] = NULL; // because my setVertexName function frees, NULL is better
   }
                      
   return g;
}

int numOfVertices(Graph g) {
   return g->nV;
}

// check if vertex is valid in a graph
int validV(Graph g, Vertex v) {
   return (g != NULL && v >= 0 && v < g->nV);
}

void insertEdge(Graph g, Edge e) {
   assert(g != NULL && validV(g,e.v) && validV(g,e.w));

   if (g->edges[e.v][e.w] == 0) {   // edge e not in graph
      g->edges[e.v][e.w] = e.weight;
      g->nE++;
   }
}

void removeEdge(Graph g, Edge e) {
   assert(g != NULL && validV(g,e.v) && validV(g,e.w));

   if (g->edges[e.v][e.w] != 0) {   // edge e in graph
      g->edges[e.v][e.w] = 0;
      g->nE--;
   }
}

int adjacent(Graph g, Vertex v, Vertex w) {
   assert(g != NULL && validV(g,v) && validV(g,w));

   return g->edges[v][w].isExist;
}

void showGraph(Graph g) {
    assert(g != NULL);
    int i, j;

    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (i = 0; i < g->nV; i++)
       for (j = 0; j < g->nV; j++)
	  if (g->edges[i][j] != 0)
	     printf("Edge %d - %d: %d\n", i, j, g->edges[i][j]);
}

void freeGraph(Graph g) {
   assert(g != NULL);

   int i;
   for (i = 0; i < g->nV; i++)
      free(g->edges[i]);
   free(g->edges);
   free(g->names);
   free(g->ferries);
   free(g);
}


/// MY FUNCTIONS /////////////////////////////////////////////////////////////////////

void setVertexName(Graph g, Vertex v, const char *name) {
   if (g->names[v] == NULL) {
      free(g->names[v]);
   }
   g->names[v] = strdup(name); // strdup is a function that mallocs and copies a string to the malloced space. Returns a pointer. 
}

int getVertexIDByName(Graph g, char *name) {
   for (int i = 0; i < g->nV; i++) {
      if (strcmp(g->names[i], name) == 0) {
         return i;
      }
   }
   // -1 denotes the landmark name was not found in the Graph.
   return -1;   
}

void insertBiDirectionalEdge(Graph g, Edge e) {
   assert(g != NULL && validV(g,e.v) && validV(g, e.w));

   Vertex v = e.v;
   Vertex w = e.w;

   if (g->edges[v][w].isExist == false ) {
      if (g->edges[w][v].isExist == false) {
         g->edges[v][w].isExist = true;
         g->edges[w][v].isExist = true;
         g->edges[v][w].walkingTime = e.weight;
         g->edges[w][v].walkingTime = e.weight;
         g->edges[v][w].isWalk = true;
         g->edges[w][v].isWalk = true;
         g->nE++; // this is 1 bidirectional edge.
      }
   }
}


//////////////////////////////////////////////////////////////////////////////////////


