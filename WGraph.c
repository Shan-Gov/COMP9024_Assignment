// Weighted Directed Graph ADT
// Adjacency Matrix Representation ... COMP9024 25T2
#include "WGraph.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stack.h"


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

// void insertEdge(Graph g, Edge e) {
//    assert(g != NULL && validV(g,e.v) && validV(g,e.w));

//    if (g->edges[e.v][e.w] == 0) {   // edge e not in graph
//       g->edges[e.v][e.w] = e.weight;
//       g->nE++;
//    }
// }

// void removeEdge(Graph g, Edge e) {
//    assert(g != NULL && validV(g,e.v) && validV(g,e.w));

//    if (g->edges[e.v][e.w] != 0) {   // edge e in graph
//       g->edges[e.v][e.w] = 0;
//       g->nE--;
//    }
// }

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
	  if (g->edges[i][j].isExist == true) {
      if(g->edges[i][j].isWalk == true) {
         printf("Walking Edge: %s - %s: %d minutes\n", g->names[i], g->names[j], g->edges[i][j].walkingTime );
      }
      if (g->edges[i][j].isFerry == true) {
         printf("Ferry Edge: %s - %s\n", g->names[i], g->names[j]);
         printf("Departing At: %d\n", g->edges[i][j].ferries->departTime);
         printf("Arriving At: %d\n", g->edges[i][j].ferries->arriveTime);
      }
     }

     // TEST ONLY | CAN DELETE
     for (int l = 0; l < numOfVertices(g); l++) {
        for (int m = 0; m < numOfVertices(g); m++) {
            if (g->edges[l][m].isExist) {
                printf("Edge from %s to %s exists\n", g->names[l], g->names[m]);
            }
        }
    }

}

void freeGraph(Graph g) {
   assert(g != NULL);

   int i;

   // Free the ferry schedules linked list
   for (i = 0; i < g->nV; i++) {
      for (int j = 0; j < g->nV; j++) {
         FerryNode *curr = g->edges[i][j].ferries;
         while(curr != NULL) {
            FerryNode *next = curr->next;
            free(curr);
            curr = next;
         }
      }
   }

   for (i = 0; i < g->nV; i++)
      free(g->edges[i]);
   free(g->edges);
   free(g->names);
   free(g);
}


/// MY FUNCTIONS /////////////////////////////////////////////////////////////////////

void setVertexName(Graph g, Vertex v, const char *name) {
   if (g->names[v] != NULL) {
      free(g->names[v]);
   }
   g->names[v] = malloc((strlen(name) + 1) * sizeof(char));
   assert(g->names[v] != NULL);
   strcpy(g->names[v], name);
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

char *getVertexNameByID(Graph g, Vertex v) {
   if (g == NULL) {
      return NULL;
   }

   char *name = g->names[v];

   return name;
}


void insertWalkingEdge(Graph g, Edge e) {
   // Walking edge is bidirectional
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


void addFerryEdge(Graph g, Vertex V, Vertex W, int departTime, int arriveTime) {
   // Ferry edge is unidirectional
   assert(g != NULL);

   FerryNode *newNode = malloc(sizeof(FerryNode));
   assert(newNode != NULL);

   newNode->departTime = departTime;
   newNode->arriveTime = arriveTime;
   newNode->next = NULL;

   FerryNode *head = g->edges[V][W].ferries;


   if (head == NULL || newNode->departTime < head->departTime) {
      // CASE 1:  Inserting at the head (empty list or earliest)
      newNode->next = head;
      g->edges[V][W].ferries = newNode;
   } else {
      // CASE 2: One Node (Insert in order)
      FerryNode *curr = g->edges[V][W].ferries;
      // 1. find the location of the insert
      while (curr->next != NULL && newNode->departTime > curr->next->departTime) {
         curr = curr->next;
      }
      // 2. perform insert
      FerryNode *oldNext = curr->next;
      curr->next = newNode;
      newNode->next = oldNext;
   }



   // Update values in struct
   g->edges[V][W].isExist = true;
   g->edges[V][W].isFerry = true;


   g->nE++; // increment the number of edges
}

// Returns the edge weight (walking time of walking edge)
int getWalkingTime(Graph g, Vertex v, Vertex w) {
   return g->edges[v][w].walkingTime;
}

// This returns a character that denotes the edge type connecting 2 vertices
char edgeType(Graph g, Vertex v, Vertex w) {
   if (g->edges[v][w].isWalk == true) {
      return 'w';
   } else if (g->edges[v][w].isFerry == true) {
      return 'f';
   }

   return 'x';
}

FerryNode *getFerrySchedule(Graph g, Vertex v, Vertex w) {
   if (g->edges[v][w].isFerry == true) {
      return g->edges[v][w].ferries;
   }

   return NULL;
}

void printFerrySchedule(Graph g, Vertex v, Vertex w) {
   FerryNode *head = g->edges[v][w].ferries;

   while (head != NULL) {
      printf("%s: %d\n", g->names[v], head->departTime);
      printf("%s: %d\n", g->names[w], head->arriveTime);
      head = head->next;
   }
}


int getTimeDifference(int t1, int t2) {
   // Extract hours and minutes
   int h1 = t1 / 100;
   int m1 = t1 % 100;

   // Extract hours and minutes
   int h2 = t2 / 100;
   int m2 = t2 % 100;

   // Find total minutes in each
   int total1 = h1 * 60 + m1; // total minutes h1
   int total2 = h2 * 60 + m2; // total minutes h2

   return total2 - total1;
}

int timeToMinutes(int t) {
   // standard conversion function
   return (t / 100) * 60 + (t % 100);
}

int minutesToTime(int mins) {
   // standard conversion function
   return (mins / 60) * 100  + (mins % 60);
}


bool dfsPathFinder(Graph g, Vertex curr, Vertex dest, int currentTimeMins, bool *visited, stack path) {

   if (curr == dest) {
      StackPush(path, curr);
      return true;
   }

   for (Vertex v = 0; v < g->nV; v++) {
      EdgeInfo edge = g->edges[curr][v];
      if(!visited[v] && edge.isExist) {
         // we check if the edge is a ferry and process accordingly
         if (edge.isFerry) {
            FerryNode *ferry = edge.ferries;
            while(ferry != NULL && timeToMinutes(ferry->departTime) < currentTimeMins) {
               // finding the right spot in the ferry schedule linked list
               ferry = ferry->next;
            }

            // recursie step (if linked list i.e. ferry schedule is not NULL/empty)
            if (ferry != NULL) {
               int arriveTimeMins = timeToMinutes(ferry->arriveTime);
               visited[v] = true;
               if (dfsPathFinder(g, v, dest, arriveTimeMins, visited, path)) {
                  StackPush(path, curr);
                  return true;
               }
               visited[v] = false;
            }
         } else if (edge.isWalk) {
            // so an edge exist but it is not a ferry. So it must be a walking edge.
            int nextTime = currentTimeMins + edge.walkingTime;

            // recursive step
            visited[v] = true;
            if (dfsPathFinder(g, v, dest, nextTime, visited, path)) {
               StackPush(path, curr);
               return true;
            }
            // if path not found, reset vertex to false so can
            // be visited again from another vertex.
            visited[v] = false;
         }
      }
   }

   return false;

}



//////////////////////////////////////////////////////////////////////////////////////
