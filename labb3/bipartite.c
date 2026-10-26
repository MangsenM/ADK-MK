/**
 * Datastructure for bipartite graph with optionaly weighted edges
 * 
 * @author Karmela Eliasson
 */

#include <stdio.h>

struct bi_graph
{
    int x;
    int y;
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
    //int capacatiy;
};

/**
 * 
 * Allocates struct for new graph
 */
struct bi_graph* init_bi_graph(int x, int y)
{
    struct bi_graph* graph = malloc(sizeof(struct bi_graph));
    struct neighbor_list* neighbor = malloc(sizeof(struct neighbor_list)*(x+y));

    struct bi_graph init = {x,y, neighbor};
    *graph = init;

    return graph;
};

/**
 *  //add inverse?
 *  //child struct file for flow or all in one?
 */
void add_edge(struct bi_graph* graph, int from, int to)
{
    struct neighbor_list* neighbors = (graph->neighbor_table)+sizeof(struct neighbor_list)*from;
};

