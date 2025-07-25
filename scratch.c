#include <AvailabilityInternalLegacy.h>
#include <stdio.h>
#include "WGraph.h"


#define MAX_NAME_LEN 31

int main(void) {
    // Get number of landmarks and create an array
    int no_landmarks;
    printf("Number of landmarks: ");
    scanf("%d", &no_landmarks);
    getchar(); // removes the \n character left in the input buffer

    Graph g = newGraph(no_landmarks);

    for (int i = 0; i < no_landmarks; i++) {
        char name[MAX_NAME_LEN];
        scanf("%s", name);
        setVertexName(g, i, name);
    }


    // Creating Walking Links //
    Edge e;
    int nLinks;
    printf("Number of walking links: ");
    scanf("%d", &nLinks);

    for(int i = 0; i < nLinks; i++) {
        char from[MAX_NAME_LEN]; // temp array to store the names scanned
        char to[MAX_NAME_LEN]; // temp array to store the names scanned
        int walking_time;
        scanf("%s", from);
        scanf("%s", to);
        scanf("%d", &walking_time);
        e.v = getVertexByName(g, from);
        e.w = getVertexByName(g, to);
        e.weight = walking_time;       
        insertBiDirectionalEdge(g, e); // bidirectional (walking) edge insertion
    }
  

    
  

}
