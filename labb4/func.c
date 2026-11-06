#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

/**struct nb_list dummy = (struct nb_list) {1, (int*) NULL, (struct nb_list*) NULL};

struct nb_list* getDummy(){
    return &dummy;
}
**/
struct graph* initGraph(int v, int e, int m){
    struct graph* graph = malloc(sizeof(struct graph));
    int** nb_mrx = calloc(v, sizeof(int*));
    int* val = calloc(v, sizeof(int));

    for (int i = 0 ; i < v ; i++){
        nb_mrx[i] = calloc(v,sizeof(int));
    }

    *graph = (struct graph) {v, e, m, val, nb_mrx};

    return graph;
};

void addEdge(struct graph* graph, int from, int to){
    if(from < to){
        if (graph->nb_mrx[from - 1][to - 1] == 0) {
            graph->val[from - 1]++;
            graph->val[to - 1]++;
        }
        graph->nb_mrx[from - 1][to - 1] = 1;
    } else {
        if (graph->nb_mrx[to - 1][from - 1] == 0) {
            graph->val[from - 1]++;
            graph->val[to - 1]++;
        }
        graph->nb_mrx[to - 1][from - 1] = 1;
    }
};

int maxVal(struct graph* graph){
    int max = 0;

    for (int i = 0; i < graph->v; i ++){
        max = max < graph->val[i] ? graph->val[i] : max;
    }

    return max;
}

int nrConnectedNodes(struct graph* graph){
    int c = 0;

    for (int i = 0 ; i < graph->v ; i++){
        if (graph->val[i] > 0){
            c++;
        }
    }

    return c;
};

void renameConnected(struct graph* graph, int* names){
    int ctr = 3; //divas are 1 and 2
    
    for(int i = 0 ; i < graph->v ; i++) {
        if (graph->val[i] != 0){
            names[i] = ctr++;
        } else {
            names[i] = 0;
        }
    }
}

void destroyGraph(struct graph* graph){
    for(int i = 0; i < graph->v; i++){
        free(graph->nb_mrx[i]);
    }
    free(graph->nb_mrx);
    free(graph->val);
    free(graph);
}