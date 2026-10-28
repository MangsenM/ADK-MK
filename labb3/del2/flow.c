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

void destroy_graph(struct flow_graph* graph){
    free(graph->neighbor_table);
    free(graph->neighbors);
    free(graph);
}

struct neighbor_list** reconstructPath(struct neighbor_list* last, int len, struct neighbor_list** visited){
    struct neighbor_list** path = malloc(sizeof(struct neighbor_list*)*len);

    path[len]=last;

    //path[0] contains min(c(p)) = flow
    for (int i = len-1 ; i >= 0 ; i--){
        path[i] = visited[path[i+1]->node];
    }

    return path;
}

struct neighbor_list** BFS(struct flow_graph* cf){
    struct neighbor_list** visited = calloc(cf->v, sizeof(struct neighbor_list));
    int* queue = calloc(cf->v, sizeof(int));
    int front = 0;
    int back = 0;
    int pathLen = 1;

    struct neighbor_list** path = NULL;

    queue[back++] = cf->s;

    while(front!=back){
        int curr = queue[front++];

        struct neighbor_list* neighbors = cf->neighbor_table[curr-1];
            
        int check = neighbors->tail==NULL ? 0 : 1;
        while(check){
            int nbr = neighbors->neighbor;
            if(visited[nbr-1]==NULL && neighbors->capacity > 0){
                
                queue[back++] = nbr;
                visited[nbr-1] = neighbors;

                pathLen++;
                if(nbr == cf->t){
                    path = reconstructPath(neighbors, pathLen, visited);
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

int findMinC(struct neighbor_list** p, int t) {
    int minc = INT_MAX;
    
    for(int i = -1; (*p)[i].neighbor != t; ++i){
        if((*p)[i].capacity < minc){
            minc = (*p)[i].capacity;
        }
    }
    
    return minc;
}
