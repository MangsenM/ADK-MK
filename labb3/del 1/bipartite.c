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
struct bi_graph* init_bi_graph(int x, int y, int e){
    struct bi_graph* graph = malloc(sizeof(struct bi_graph));
    struct neighbor_list* neighbor = malloc(sizeof(struct neighbor_list)*(x+y+2*e));

    for(int i = 0; i < x+y-1 ; i++){
        *(neighbor+i) = (struct neighbor_list) {};
    }

    *graph = (struct bi_graph) {x, y, e, neighbor};

    return graph;
};

/**
 *  Adds edges during initilization, edge is added as neighbor (x to y) and inverse (y to x)
 */
void init_add_edge(struct bi_graph* graph, struct neighbor_list* forwards, struct neighbor_list* backwards, int from, int to){
    struct neighbor_list** neighbors_from = (graph->neighbor_table)+from-1;
    struct neighbor_list** neighbors_to = (graph->neighbor_table)+to-1;
    
    //update head of neighborlist with updated previous for last head
    *forwards = (struct neighbor_list) {to,-1,backwards,*neighbors_from};
    *neighbors_from = forwards;

    *backwards = (struct neighbor_list) {from,-1,forwards,*neighbors_to};
    *neighbors_to = backwards;
};

void destroy_graph(struct bi_graph* graph){
    free(graph->neighbor_table);
    free(graph);
}