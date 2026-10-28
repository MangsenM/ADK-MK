#ifndef GRAPHUTIL_H
#define GRAPHUTIL_H

/* Graph data structure */
struct flow_graph
{
    int v;
    int e;
    int s;
    int t;
    int f;
    struct neighbor_list** neighbor_table; //allocate x+y list heads
    struct neighbor_list* neighbors;
};

struct neighbor_list
{
    int node;
    int neighbor;
    int capacity;
    struct neighbor_list* inverse;
    struct neighbor_list* tail;
};

/* Graph functions */
struct flow_graph* init_flow_graph(int v, int e, int s, int t);
void init_add_edge(struct flow_graph* graph, struct neighbor_list* edges, struct neighbor_list* invedges, int from, int to, int capacity);
void destroy_graph(struct flow_graph* graph);
struct neighbor_list** BFS(struct flow_graph* cf);
int findMinC(struct neighbor_list** p, int t);

#endif
