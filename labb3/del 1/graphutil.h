#ifndef GRAPHUTIL_H
#define GRAPHUTIL_H

/* Graph data structure */
struct bi_graph
{
    int x;
    int y;
    int e;
    struct neighbor_list** neighbor_table; //allocate x+y list heads
    struct neighbor_list* neighbors;
};

struct neighbor_list
{
    int neighbor;
    int capacatiy;
    struct neighbor_list* inverse;
    struct neighbor_list* tail;
};

/* Graph functions */
struct bi_graph* init_bi_graph(int x, int y, int e);
void init_add_edge(struct bi_graph* graph, struct neighbor_list* edgess, struct neighbor_list* invedgess, int from, int to);
void destroy_graph(struct bi_graph* graph);

#endif
