#ifndef GRAPHUTIL_H
#define GRAPHUTIL_H

/* Graph data structure */
struct bi_graph
{
    int x;
    int y;
    int e;
    struct neighbor_list* neighbor_table; //allocate x+y list heads
};

struct neighbor_list
{
    struct edge* ptr;
    struct edge* inv_ptr;
    struct neighbor_list* next;
    struct neighbor_list* prev;
};

struct edge
{
    int to;
    int capacatiy;
};

/* Graph functions */
struct bi_graph* init_bi_graph(int x, int y, int e);
void add_edge(struct bi_graph* graph, int from, int to);

#endif