/**
 * Functions for initializing and calculating flow in flow datastructures
 * See graphutil.h for structure implementation
 * 
 * @author Karmela Eliasson and Malcolm Mångsén
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "graphutil.h"

static struct edge dummy = (struct edge) {0,0,0,NULL,NULL};

/** 
 * Allocates struct for new flow graph
 * Specific edges added afterwards
 * 
 * @param v number of vertseries
 * @param e number of edges
 * @param s source
 * @param t target
 */
struct flow_graph* initFlowGraph(int v, int e, int s, int t){
    struct flow_graph* graph = malloc(sizeof(struct flow_graph));
    struct edge** neighbor_table = calloc(v, sizeof(struct edge*));
    struct edge* edge = calloc(2*e, sizeof(struct edge));

    *graph = (struct flow_graph) {v, e, s, t, 0, neighbor_table, edge};

    return graph;
};

/**
 * Initialization for edges in graph, one edge at a time
 * 
 * @param graph graph to add edge to
 * @param edge pointer to location where edge should be stored
 * @param invedge pointer to location where inverted edge should be stored
 * @param from node where edge starts
 * @param to node where edge terminates
 * @param capacity capacity of edge
 */
void initEdge(struct flow_graph* graph, struct edge* edge, struct edge* invedge, int from, int to, int capacity){
    struct edge** neighbors_from = (graph->neighbor_table)+from-1; // -1 as edges 1 indexed
    struct edge** neighbors_to = (graph->neighbor_table)+to-1;
   
    //special handling for first edge from node
    if ((*neighbors_from) == NULL){
        *edge = (struct edge) {from, to,capacity, invedge, NULL};
        (graph->neighbor_table)[from - 1] = edge;
    } else {
        *edge = (struct edge) {from, to,capacity, invedge, *neighbors_from};
        (graph->neighbor_table)[from - 1] = edge;
    }

    if ((*neighbors_to) == NULL){
        *invedge = (struct edge) {to, from,0,edge, NULL};
        (graph->neighbor_table)[to - 1] = invedge;
    } else {
        *invedge = (struct edge) {to, from,0,edge,*neighbors_to};
        (graph->neighbor_table)[to - 1] = invedge;
    }
};

/**
 * Breath first search including check for capacity in reversed edges
 * used for finding path to increase flow
 * 
 * @param graph flow graph where search is conducted
 * @param visited visited list storing edges *also used as search tree*
 */
int BFS(struct flow_graph* graph, struct edge** visited){
    int* queue = calloc(graph->v, sizeof(int));
    int front = 0; //pointers for knowing where to dequeue
    int back = 0; //and queue

    queue[back++] = graph->s;
    visited[graph->s-1] = &dummy; //dummy used to not revisit source

    //if front == back all accessible visited (t inaccessible)
    while(front!=back){
        int curr = queue[front++];

        struct edge* neighbor_edge;
        int check = 1;

        if (graph->neighbor_table[curr-1] == NULL){ //check has neighbors
            check = 0;
        } else {
            neighbor_edge = graph->neighbor_table[curr-1];
        }

        while(check){
            int nbr = neighbor_edge->to;
            if(visited[nbr-1]==NULL && neighbor_edge->capacity > 0){ //can traverse
                
                queue[back++] = nbr;
                visited[nbr-1] = neighbor_edge;

                if(nbr == graph->t){ //found target
                    free(queue);
                    return 1;
                }
            }
            if(neighbor_edge->next==NULL){
                check = 0;
            } else {
                neighbor_edge = neighbor_edge->next;
            }
        }
    }

    free(queue);
    return 0;
}

/**
 * Reconstructs path from source to target using search tree (visited array)
 * 
 * @param search_tree array containing edge used to get to node by index (-1)
 * @param path allocation for storing path edges (correct length)
 * @param len length of path
 * @param target last node in path
 */
void reconstructPath(struct edge** search_tree, struct edge** path, int len, int target){
    path[len-1]=search_tree[target-1]; //setup last edge to be able to iteratively back traverse

    for (int i = len-2 ; i >= 0 ; i--){
        path[i] = search_tree[path[i+1]->from-1]; //edge in search tree where last edge was from
    }
}

/**
 * Gets length and the lowest capacity in path
 * 
 * @param search_tree array containing edge used to get to node by index (-1)
 * @param lenAminC array to store [length, min capacity]
 * @param s source
 * @param t target
 */
void findLenAndMinC(struct edge** search_tree, int* lenAminC, int s, int t) {
    lenAminC[0] = 0;
    lenAminC[1] = INT_MAX; //capacities will always be lower

    struct edge* curr = search_tree[t - 1]; //start at target, iterate to s

    int check = 1;
    while(check){
        lenAminC[0]++; //increase path length

        if(curr->capacity < lenAminC[1]){
            lenAminC[1] = curr->capacity; //update c if new min
        }
        
        if(curr->from == s){ //terminate when source found
            check = 0;
        } else{ // go to previous edge in path
            curr = search_tree[curr->from - 1];
        }
    }
}

/**
 * Deallocates all structures in flow graph
 * 
 * @param graph To destroy
 */
void destroy_flow_graph(struct flow_graph* graph){
    free(graph->neighbor_table);
    free(graph->edges);
    free(graph);
}