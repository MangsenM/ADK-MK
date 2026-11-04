#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

struct graph* initGraph(int v, int e, int m){
    struct graph* graph = malloc(sizeof(struct graph));
    struct nb_list** nb_table = calloc(v, sizeof(struct nb_list*));
    struct nb_list* edges = calloc(e, sizeof(struct nb_list));

    *graph = (struct graph) {v, e, m, nb_table, edges};

    return graph;
};

void initEdge(struct graph* graph, struct nb_list* edge, int from, int to){
    struct nb_list** nb_from = (graph->nb_table)+from-1; // -1 as edges 1 indexed
   
    //special handling for first edge from node
    if ((*nb_from) == NULL){
        *edge = (struct nb_list) {1, to, NULL};
        (graph->nb_table)[from - 1] = edge;
    } else {
        *edge = (struct nb_list) {graph->nb_table[from - 1]->length + 1, to, graph->nb_table[from -1]};
        (graph->nb_table)[from - 1] = edge;
    }
};

