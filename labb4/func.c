#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

struct nb_list dummy = (struct nb_list) {1, (int*) NULL, (struct nb_list*) NULL};

struct nb_list* getDummy(){
    return &dummy;
}

struct graph* initGraph(int v, int e, int m){
    struct graph* graph = malloc(sizeof(struct graph));
    struct nb_list** nb_table = calloc(v, sizeof(struct nb_list*));
    struct nb_list* edges = calloc(e, sizeof(struct nb_list));
    int* names = calloc(v,sizeof(int));

    for (int i = 0 ; i <=v ; i++){
        names[i] = i+1;
    }

    *graph = (struct graph) {v, e, m, names, nb_table, edges};

    return graph;
};

void initEdge(struct graph* graph, struct nb_list* edge, int from, int to){
    struct nb_list** nb_from = (graph->nb_table)+from-1; // -1 as edges 1 indexed
    struct nb_list** nb_to = (graph->nb_table)+to-1;

    //special handling for first edge from node
    if ((*nb_from) == NULL || (*nb_from) == &dummy){
        *edge = (struct nb_list) {1, graph->names+to, NULL};
        (graph->nb_table)[from - 1] = edge;
    } else {
        *edge = (struct nb_list) {graph->nb_table[from - 1]->length + 1, graph->names+to, graph->nb_table[from -1]};
        (graph->nb_table)[from - 1] = edge;
    }

    if((*nb_to) == NULL){
        (graph->nb_table)[to - 1] = &dummy;
    }
};

void rmIsolatedNodes(struct graph* graph){
    int nr_removed = 0;
    int v_updated = graph->v;

    for(int i = 0 ; i < graph->v ; i++){
        if(graph->nb_table[i] == NULL){
            nr_removed++;
            v_updated--;
            graph->nb_table[i] = NULL;
        } else {
            graph->names[i]-= nr_removed;
        }
    }

    graph->v = v_updated;
}