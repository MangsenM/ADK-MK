/**
 * Datastructure for bipartite graph with optionaly weighted edges
 * 
 * @author Karmela Eliasson and Malcolm Mangsén
 */

#include <stdio.h>
#include "graphutil.h"

/**
 * 
 * Allocates struct for new graph
 */
struct bi_graph* init_bi_graph(int x, int y, int e)
{
    struct bi_graph* graph = malloc(sizeof(struct bi_graph));
    struct neighbor_list* neighbor = malloc(sizeof(struct neighbor_list)*(x+y));

    *graph = (struct bi_graph) {x, y, e, neighbor};

    return graph;
};

/**
 *  //add inverse?
 *  //child struct file for flow or all in one?
 */
void add_edge(struct bi_graph* graph, int from, int to)
{
    struct neighbor_list* neighbors_from = (graph->neighbor_table)+sizeof(struct neighbor_list)*(from-1);
    struct neighbor_list* neighbors_to = (graph->neighbor_table)+sizeof(struct neighbor_list)*(to-1);

    struct edge* fromto = malloc(sizeof(struct edge));
    struct edge* tofrom = malloc(sizeof(struct edge));

    *fromto = (struct edge) {to};
    *tofrom = (struct edge) {from};
    
    struct neighbor_list* forwards = malloc(sizeof(struct neighbor_list));
    struct neighbor_list* backwards = malloc(sizeof(struct neighbor_list));
    
    //update head of neighborlist with updated previous for last head
    *forwards = (struct neighbor_list) {fromto, tofrom, neighbors_from, NULL};
    neighbors_from->prev = forwards;
    neighbors_from = forwards;

    *backwards = (struct neighbor_list) {fromto, tofrom, neighbors_to, NULL};
    neighbors_to->prev = backwards;
    neighbors_to = backwards;
};



