#ifndef GRAPH_H
#define GRAPH_H

struct graph{
    int v;
    int e;
    int m;
    struct nb_list** nb_table;
    struct nb_list* edges;
};

struct nb_list{
    int length;
    int nb;
    struct nb_list* tail;
};

struct graph* initGraph(int v, int e, int m);
void initEdge(struct graph* graph, struct nb_list* edge, int from, int to);

#endif


