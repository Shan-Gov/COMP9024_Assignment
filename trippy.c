// LIBRARIES //
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include "WGraph.h"
#include <stdbool.h>
#include <string.h>

#define WALK w
#define FERRY f
#define MIN_TIME 0000
#define MAX_TIME 2359

typedef int TIME;


// DEFINITIONS //
#define MAX_NAME_LEN 32

// FUNCTION PROTOTYPES //


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
        insertBiDirectionalEdge(g, e); // bidirectional (walking) edge insertion
    }

    int nFerries;
    printf("Number of ferry schedules: ");
    scanf("%d", &nFerries);
    setNoFerries(g, nFerries);
    

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
        
        setFerrySchedules(g, i, depart, arrive, departTime, arriveTime);
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
    
    }
    
    
    showGraph(g);
    
    return 0;
}


// FUNCTION IMPLEMENTATIONS //

void print_landmarks(char landmarks[][MAX_NAME_LEN], int no_landmarks, int max_cols) {
    for (int i = 0; i < no_landmarks; i++) {
        printf("Landmark no %d: ", i+1);
        for (int j = 0; j < MAX_NAME_LEN && landmarks[i][j] != '\0'; j++) {
            printf("%c", landmarks[i][j]);
        }
    }
}



bool isValidTime(TIME t) {
    // this evaluates the expression and returns true or false. t % 100
    // checks is the minutes are less than 60. credit: extract_last_two_digits
    return (t >= MIN_TIME && t <= MAX_TIME && t % 100 < 60);  
}
