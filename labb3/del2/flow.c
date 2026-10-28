/**
 * Datastructure for bipartite graph with optionaly weighted edges
 * 
 * @author Karmela Eliasson and Malcolm Mangsén
 */

#include <stdio.h>
#include <stdlib.h>
#include "graphutil.h"

/**
 * 
 * Allocates struct for new graph
 */
struct flow_graph* init_bi_graph(int v, int e, int s, int t){
    struct flow_graph* graph = malloc(sizeof(struct flow_graph));
    struct neighbor_list** neighbor_table = calloc(v, sizeof(struct neighbor_list*));
    struct neighbor_list* neighbor_list = calloc(2*e, sizeof(struct neighbor_list));

    *graph = (struct flow_graph) {v, e, s, t, neighbor_table, neighbor_list};

    return graph;
};

void init_add_edge(struct flow_graph* graph, struct neighbor_list* edge, struct neighbor_list* invedge, int from, int to, int capasity){
    struct neighbor_list** neighbors_from = (graph->neighbor_table)+from-1;
    struct neighbor_list** neighbors_to = (graph->neighbor_table)+to-1;
   
    if ((*neighbors_from) == NULL){
        *edge = (struct neighbor_list) {to,capasity, invedge, NULL};
        (graph->neighbor_table)[from - 1] = edge;
    } else {
        *edge = (struct neighbor_list) {to,capasity, invedge, *neighbors_from};
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

int* BFS(struct flow_graph* cf){
    int* visited = calloc(cf->v, sizeof(int));
    int* queue = calloc(cf->v, sizeof(int));
    int front = 0;
    int back = 0;
    int pathLen = 0;

    int* path = NULL;

    queue[back++] = cf->s;
    visited[cf->s] = -1;

    while(front!=back){
        int curr = queue[front++];

        struct neighbor_list* neighbors = cf->neighbor_table[curr-1];
            
        while(neighbors->tail!=NULL){
            int nbr = neighbors->neighbor;
            if(visited[nbr-1]==0 && neighbors->capacatiy > 0){
                queue[back++] = nbr;
                visited[nbr-1] = curr;
                if(nbr == cf->t){
                    path = reconstructPath(cf->s,cf->t,visited);
                }
            }
        }
    }

    free(visited);
    free(queue);

    return path;
}

int* reconstructPath(int s, int t, int* visited){
    /**
     * start at t and walk check parent[t] to get precious node
     */
}

int minCapasityPath();