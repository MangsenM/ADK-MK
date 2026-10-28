#ifndef GRAPHUTIL_H
#define GRAPHUTIL_H

/* Graph data structure */
struct flow_graph
{
    int v;
    int e;
    int s;
    int t;
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
struct flow_graph* init_flow_graph(int x, int y, int e);
void init_add_edge(struct flow_graph* graph, struct neighbor_list* edgess, struct neighbor_list* invedgess, int from, int to, int capasity);
void destroy_graph(struct flow_graph* graph);

#endif
