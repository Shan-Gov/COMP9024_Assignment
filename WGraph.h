// Weighted Graph ADT interface ... COMP9024 25T2

#ifndef WGRAPH_H
#define WGRAPH_H
#define NOT_WALKABLE 2147483647
#define NO_FERRY -2147483647

typedef struct GraphRep *Graph;

typedef struct FerryNode {
    int departTime;
    int arriveTime;
    struct FerryNode *next;
} FerryNode;

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

#endif
