/**
 * Datastructure for bipartite graph with optionaly weighted edges
 * 
 * @author Karmela Eliasson and Malcolm Mangsén
 */

#include <stdlib.h>
#include "graphutil.h"

/**
 * 
 * Allocates struct for new graph
 */
struct bi_graph* init_bi_graph(int x, int y, int e){
    struct bi_graph* graph = malloc(sizeof(struct bi_graph));
    struct neighbor_list** neighbor_table = calloc(x + y, sizeof(struct neighbor_list**));
    struct neighbor_list* neighbor_list = calloc(2*e, sizeof(struct neighbor_list));

    *graph = (struct bi_graph) {x, y, e, neighbor_table, neighbor_list};

    return graph;
};

/**
 *  Adds edges during initilization, edge is added as neighbor (x to y) and inverse (y to x)
 */
void init_add_edge(struct bi_graph* graph, struct neighbor_list* edges, struct neighbor_list* invedges, int from, int to){
    struct neighbor_list** neighbors_from = (graph->neighbor_table)+from-1;
    struct neighbor_list** neighbors_to = (graph->neighbor_table)+to-1;
   
    if ((*neighbors_from)->neighbor == 0){
    
        *edges = (struct neighbor_list) {to,1, invedges, NULL};
        *neighbors_from = edges;

        *invedges = (struct neighbor_list) {from,-1,edges, NULL};
        *neighbors_to = invedges;

        return;
    }
    //update head of neighborlist with updated previous for last head
    *edges = (struct neighbor_list) {to,1, invedges, *neighbors_from};
    *neighbors_from = edges;

    *invedges = (struct neighbor_list) {from,-1,edges,*neighbors_to};
    *neighbors_to = invedges;
};

void destroy_graph(struct bi_graph* graph){
    free(graph->neighbor_table);
    free(graph->neighbors);
    free(graph);
}
