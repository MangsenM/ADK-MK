/**
 * Datastructure for bipartite graph with optionaly weighted edges
 * 
 * @author Karmela Eliasson and Malcolm Mångsén
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "graphutil.h"

static struct neighbor_list dummy = (struct neighbor_list) {0,0,0,NULL,NULL};

/**
 * 
 * Allocates struct for new graph
 */
struct flow_graph* init_flow_graph(int v, int e, int s, int t){
    struct flow_graph* graph = malloc(sizeof(struct flow_graph));
    struct neighbor_list** neighbor_table = calloc(v, sizeof(struct neighbor_list*));
    struct neighbor_list* neighbor_list = calloc(2*e, sizeof(struct neighbor_list));

    *graph = (struct flow_graph) {v, e, s, t, 0, neighbor_table, neighbor_list};

    return graph;
};

void init_add_edge(struct flow_graph* graph, struct neighbor_list* edge, struct neighbor_list* invedge, int from, int to, int capacity){
    struct neighbor_list** neighbors_from = (graph->neighbor_table)+from-1;
    struct neighbor_list** neighbors_to = (graph->neighbor_table)+to-1;
   
    if ((*neighbors_from) == NULL){
        *edge = (struct neighbor_list) {from, to,capacity, invedge, NULL};
        (graph->neighbor_table)[from - 1] = edge;
    } else {
        *edge = (struct neighbor_list) {from, to,capacity, invedge, *neighbors_from};
        (graph->neighbor_table)[from - 1] = edge;
    }

    if ((*neighbors_to) == NULL){
        *invedge = (struct neighbor_list) {to, from,0,edge, NULL};
        (graph->neighbor_table)[to - 1] = invedge;
    } else {
        *invedge = (struct neighbor_list) {to, from,0,edge,*neighbors_to};
        (graph->neighbor_table)[to - 1] = invedge;
    }
};

int BFS(struct flow_graph* cf, struct neighbor_list** visited){
    int* queue = calloc(cf->v, sizeof(int));
    int front = 0;
    int back = 0;

    queue[back++] = cf->s;
    visited[cf->s-1] = &dummy;

    while(front!=back){
        int curr = queue[front++];

        struct neighbor_list* neighbors;
        int check = 1;

        if (cf->neighbor_table[curr-1] == NULL){
            check = 0;
        } else {
            neighbors = cf->neighbor_table[curr-1];
        }

        while(check){
            int nbr = neighbors->neighbor;
            if(visited[nbr-1]==NULL && neighbors->capacity > 0){
                
                queue[back++] = nbr;
                visited[nbr-1] = neighbors;

                if(nbr == cf->t){
                    free(queue);
                    return 1;
                }
            }
            if(neighbors->tail==NULL){
                check = 0;
            } else {
                neighbors = neighbors->tail;
            }
        }
    }

    free(queue);
    return 0;
}

void reconstructPath(struct neighbor_list** visited, struct neighbor_list** path, int len, int last){
    path[len-1]=visited[last-1];

    for (int i = len-2 ; i >= 0 ; i--){
        path[i] = visited[path[i+1]->node-1];
    }
}

void findLenAndMinC(struct neighbor_list** visited, int* lenAminC, int s, int t) {
    lenAminC[0] = 0;
    lenAminC[1] = INT_MAX;

    struct neighbor_list* curr = visited[t - 1];

    int check = 1;
    while(check){
        lenAminC[0]++; //increse path length

        if(curr->capacity < lenAminC[1]){
            lenAminC[1] = curr->capacity; //update c if new min
        }
        
        if(curr->node == s){
            check = 0;
        } else{
            curr = visited[curr->node - 1];
        }
    }
}
