#ifndef GRAPH_H
#define GRAPH_H

struct graph{
    int v;
    int max;
    int e;
    int m;
    int s;
    int* val;
    struct edge** nb_table;
};

struct edge{
    int to;
    int from;
};

struct graph* initGraph(int v, int e, int m);
void initEdge(struct graph* graph, struct nb_list* edge, int from, int to);
void rmIsolatedNodes(struct graph* graph);
void rmDupEdges(struct graph* graph);
struct nb_list* getDummy();

#endif


