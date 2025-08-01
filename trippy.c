// LIBRARIES //
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include "WGraph.h"
#include <stdbool.h>
#include <string.h>
#include "queue.h"
#include "stack.h"


// MACROS //
#define MIN_TIME 0000
#define MAX_TIME 2359
#define MAX_NAME_LEN 32

// DEFINITIONS //
typedef int TIME;


// typedef struct *TIME {
//     int hours;
//     int minutes;
// } TIME;


// FUNCTION PROTOTYPES //
bool isValidTime(TIME t);
void printingPath(Graph g, int *prev, Vertex from, Vertex to);

//// MAIN ////
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
        e.v = getVertexIDByName(g, from);
        e.w = getVertexIDByName(g, to);
        e.weight = walking_time;
        insertWalkingEdge(g, e); // bidirectional (walking) edge insertion
    }

    int nFerries;
    printf("Number of ferry schedules: ");
    scanf("%d", &nFerries);
    // setNoFerries(g, nFerries);


    for (int i = 0; i < nFerries; i++) {
        char depart[MAX_NAME_LEN];
        char arrive[MAX_NAME_LEN];
        int departTime;
        int arriveTime;
        while (true) {
            scanf("%s", depart);
            getchar();
            scanf("%d", &departTime);
            if (departTime < MIN_TIME || departTime > MAX_TIME) {
                printf("Bad input. Try again\n");
                continue;
            } // will go back to the start of the loop and rescan everything
            scanf("%s", arrive);
            getchar();
            scanf("%d", &arriveTime);
            if (arriveTime < MIN_TIME || departTime > MAX_TIME || arriveTime <= departTime) {
                printf("Bad input. Try again.\n");
                continue;
            }
            break; // break out of while loop. All input valid
        }

        Vertex departID = getVertexIDByName(g, depart);
        Vertex arriveID = getVertexIDByName(g, arrive);


        if (departID == -1 || arriveID == -1) {
            printf("Error: Invalid landmark name.\n");
            continue;
        }
        addFerryEdge(g, departID, arriveID, departTime, arriveTime);
    } // for loop close


    char user_query_from[MAX_NAME_LEN];
    char user_query_to[MAX_NAME_LEN];
    TIME departing_at;

    while (true) {
        printf("From: ");
        scanf("%s", user_query_from);
        if (strcmp(user_query_from, "done") == 0) {
            break;
        }
        printf("To: ");
        scanf("%s", user_query_to);
        printf("Departing at: ");
        scanf("%d", &departing_at);

        Vertex from_dest = getVertexIDByName(g, user_query_from);
        Vertex to_dest = getVertexIDByName(g, user_query_to);

        bool is_adjacent = adjacent(g, from_dest, to_dest);
        int nV = numOfVertices(g);
        int prev[nV]; // moved this here because will need it accessible later.
        bool visited[nV];

        bool pathFound = false;
        stack path = newStack();


        if (is_adjacent == true) {
            if (edgeType(g, from_dest, to_dest) == 'w') {
                // Do if edge is a walking edge.
                int walking_time = getWalkingTime(g, from_dest, to_dest);
                if (walking_time != NOT_WALKABLE) {
                    printf("Walk %d minute(s):\n", walking_time);
                    printf("  %04d %s\n", departing_at, user_query_from);
                    printf("  %04d %s\n", departing_at + walking_time, user_query_to);
                }
            }
            if (edgeType(g, from_dest, to_dest) == 'f') {
                FerryNode *ferry_schedule = getFerrySchedule(g, from_dest, to_dest);
                if (ferry_schedule != NULL) {
                    FerryNode *curr = ferry_schedule;
                    while(curr->departTime < departing_at) {
                        curr = curr->next;
                    }
                    printf("Ferry %d minute(s):\n", getTimeDifference(curr->departTime, curr->arriveTime));
                    printf("  %04d %s\n", curr->departTime, user_query_from);
                    printf("  %04d %s\n", curr->arriveTime, user_query_to);
                }
            }
        } else {
            // Initialise Arrays for BFS (now DFS)
            for (int i = 0; i < nV; i++) {
                visited[i] = false;
                prev[i] = -1;
            }

            // queue q = newQueue();
            // QueueEnqueue(q, from_dest);
            // visited[from_dest] = true;

            // BFS Search Algorithm using queue
            // while (!QueueIsEmpty(q)) {
            //     int v = QueueDequeue(q);
            //     for (int w = 0; w < nV; w++) {
            //         if (adjacent(g,v,w) && visited[w] == false) {
            //             QueueEnqueue(q, w);
            //             visited[w] = true;
            //             prev[w] = v;
            //         }
            //     }
            // }

            int startingTimeMins = timeToMinutes(departing_at);

            pathFound = dfsPathFinder(g, from_dest, to_dest, startingTimeMins, visited, path);
        }

        if (pathFound == false) {
            printf("No available route.\n");
        } else {
            // path is in the stack
            int currentTime = departing_at;

            Vertex prev = StackPop(path);
            printf("From: %s\n", getVertexNameByID(g, prev));
            printf("To: %s\n", getVertexNameByID(g, to_dest));
            printf("Departing at: %04d\n\n", departing_at);

            while (!StackIsEmpty(path)) {
                Vertex curr = StackPop(path);

                EdgeInfo edge = g->edges[prev][curr];

                if (edge.isWalk) {
                    printf("Walk %d minute(s):\n", edge.walkingTime);
                    printf("  %04d %s\n", currentTime, getVertexNameByID(g, prev));
                    currentTime = timeToMinutes(currentTime);
                    currentTime += edge.walkingTime;
                    currentTime = minutesToTime(currentTime);
                    printf("  %04d %s\n", currentTime, getVertexNameByID(g, curr));
                } else if (edge.isFerry) {
                    FerryNode *ferry = edge.ferries;
                    while(ferry != NULL && timeToMinutes(ferry->departTime) < timeToMinutes(currentTime)) {
                        ferry = ferry->next;
                    }
                    if (ferry != NULL) {
                        printf("Ferry %d minute(s):\n", getTimeDifference(ferry->departTime, ferry->arriveTime));
                        printf("  %04d %s\n", ferry->departTime, getVertexNameByID(g, prev));
                        currentTime = ferry->arriveTime;
                        printf("  %04d %s\n", ferry->arriveTime, getVertexNameByID(g, curr));
                    }
                }

                prev = curr;
            }




            // stack s = newStack();
            // StackPush(s, to_dest);
            // Vertex v = prev[to_dest];
            // while (v != -1) {
            //     StackPush(s, v);
            //     v = prev[v];
            // }


            // TIME current_time = departing_at;
            // Vertex previous = StackPop(s);

            // // Printing Path
            // while (!StackIsEmpty(s)) {
            //     Vertex curr = StackPop(s);
            //     int walking_time = getWalkingTime(g, previous, curr);
            //     printf("Walk %d minute(s)\n", walking_time);
            //     printf("  %04d %s\n", current_time, getVertexNameByID(g, previous));
            //     current_time += walking_time;
            //     printf("  %04d %s\n", current_time, getVertexNameByID(g, curr));

            //     previous = curr;
            // }
        }

    }

    printf("\n");
    printf("\n");
    printf("==================================CHECKING ALL GRAPH EDGES==================================\n");
    showGraph(g);
    printf("\n");
    printf("\n");


    printf("Safe travels!\n");
    return 0;
}


// FUNCTION IMPLEMENTATIONS //

bool isValidTime(TIME t) {
    // this evaluates the expression and returns true or false. t % 100
    // checks is the minutes are less than 60. credit: extract_last_two_digits
    return (t >= MIN_TIME && t <= MAX_TIME && t % 100 < 60);
}


