/**
 * Datastructure for bipartite graph with optionaly weighted edges
 * 
 * @author Karmela Eliasson and Malcolm Mångsén
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "graphutil.h"
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
        *edge = (struct neighbor_list) {to,capacity, invedge, NULL};
        (graph->neighbor_table)[from - 1] = edge;
    } else {
        *edge = (struct neighbor_list) {to,capacity, invedge, *neighbors_from};
        (graph->neighbor_table)[from - 1] = edge;
    }

    if ((*neighbors_to) == NULL){
        *invedge = (struct neighbor_list) {from,0,edge, NULL};
        (graph->neighbor_table)[to - 1] = invedge;
    } else {
        *invedge = (struct neighbor_list) {from,0,edge,*neighbors_to};
        (graph->neighbor_table)[to - 1] = invedge;
    }
};

void destroy_graph(struct flow_graph* graph){
    free(graph->neighbor_table);
    free(graph->neighbors);
    free(graph);
}

int* reconstructPath(int t, int len, int* visited){
    int* path = malloc(sizeof(int)*len + 1);

    path[len]=t;

    //path[0] contains min(c(p)) = flow
    for (int i = len-1 ; i >= 0 ; i--){
        path[i] = visited[path[i+1]];
    }

    return path;
}

int* BFS(struct flow_graph* cf){
    int* visited = calloc(cf->v, sizeof(int));
    int* queue = calloc(cf->v, sizeof(int));
    int front = 0;
    int back = 0;
    int pathLen = 1;

    int* path = NULL;

    queue[back++] = cf->s;
    visited[cf->s] = INT_MAX;

    while(front!=back){
        int curr = queue[front++];

        struct neighbor_list* neighbors = cf->neighbor_table[curr-1];
            
        int check = 1;
        if(neighbors->tail==NULL){
                check = 0;
        }
        while(check){
            int nbr = neighbors->neighbor;
            if(visited[nbr-1]==0 && neighbors->capacity > 0){
                
                visited[cf->s] = visited[cf->s] > neighbors->capacity ? neighbors->capacity : visited[cf->s];
                queue[back++] = nbr;
                visited[nbr-1] = curr;

                pathLen++;
                if(nbr == cf->t){
                    path = reconstructPath(cf->t, pathLen, visited);
                }
            }
            if(neighbors->tail==NULL){
                check = 0;
            } else {
                neighbors = neighbors->tail;
            }
        }
    }

    free(visited);
    free(queue);

    return path;
}
