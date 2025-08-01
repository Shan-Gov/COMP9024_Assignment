// Weighted Graph ADT interface ... COMP9024 25T2

#ifndef WGRAPH_H
#define WGRAPH_H

#define NOT_WALKABLE 2147483647
#define NO_FERRY -2147483647

#include <stdbool.h>
#include "stack.h"

typedef struct GraphRep *Graph;

// Ferry schedule struct
typedef struct FerryNode {
    int departTime;
    int arriveTime;
    struct FerryNode *next;
} FerryNode;


// A struct to hold all the required edge info
typedef struct EdgeInfo {
   bool isExist;
   bool isWalk;
   int walkingTime;
   bool isFerry;
   int departTime;
   int arriveTime;
   FerryNode *ferries; // a linked list of the ferry schedules. depart @ v & arrive @ w
} EdgeInfo;

// Forward declaring because I can't dfs to work without it. Should be private in WGraph.c
// Use sparingly.
typedef struct GraphRep {
   EdgeInfo **edges;  // Adjusted to store struct EdgeInfo which contains all edge info.
   int nV;       // #vertices
   int nE;       // #edges
   char **names; // Names of the landmarks will go here 2D array
} GraphRep;


// vertices are ints
typedef int Vertex;

// edges are pairs of vertices (end-points) with a weight
typedef struct Edge {
   Vertex v;
   Vertex w;
   int    weight;
} Edge;

Graph newGraph(int);
int   numOfVertices(Graph);
void  insertEdge(Graph, Edge);
void  removeEdge(Graph, Edge);
int   adjacent(Graph, Vertex, Vertex);  // returns weight, or 0 if not adjacent
void  showGraph(Graph);
void  freeGraph(Graph);
void setVertexName(Graph g, Vertex v, const char *name);
int getVertexIDByName(Graph g, char *name);
char *getVertexNameByID(Graph g, Vertex v);
void addFerryEdge(Graph g, Vertex V, Vertex W, int departTime, int arriveTime);
void insertWalkingEdge(Graph g, Edge e);
int getWalkingTime(Graph g, Vertex v, Vertex w);
char edgeType(Graph g, Vertex v, Vertex w);
FerryNode *getFerrySchedule(Graph g, Vertex v, Vertex w);
void printFerrySchedule(Graph g, Vertex v, Vertex w);
int getTimeDifference(int t1, int t2);
int timeToMinutes(int t);
int minutesToTime(int mins);
bool dfsPathFinder(Graph g, Vertex curr, Vertex dest, int currentTimeMins, bool *visited, stack path);

#endif
