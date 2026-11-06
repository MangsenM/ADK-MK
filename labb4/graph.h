#ifndef GRAPH_H
#define GRAPH_H

struct graph{
    int v;
    int e;
    int m;
    int* val;
    int* belonging_edges;
    int** nb_mrx;
};

struct graph* initGraph(int v, int e, int m);
void addEdge(struct graph* graph, int from, int to);
int maxVal(struct graph* graph);
int nrConnectedNodes(struct graph* graph);
void renameConnected(struct graph* graph, int* names);
void destroyGraph(struct graph* graph);
int THEEVERYTHINGFIX(struct graph* graph);

#endif


