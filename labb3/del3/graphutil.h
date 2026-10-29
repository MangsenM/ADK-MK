/**
 * Structures for storing bipartite and flowgraphs with related functions
 * 
 * @author Karmela Eliasson and Malcolm Mangsen
 */


#ifndef GRAPHUTIL_H
#define GRAPHUTIL_H

/**
 * Structure for bipartite graph
 * 
 * x,y number of nodes in x and y respectivly
 * e number of edges
 * *edges array of edges stored [to, from], each edge takes two positions
 */
struct bi_graph {
  int x;
  int y;
  int e;
  int *edges;
};

/**
 * Structure for graphs with edge capacaties
 * 
 * v number of vertesies
 * e number of edges
 * s source
 * t target
 * f calculated flow
 * neighbor_table each index stores linked list of edges connected to the node
 * edges list of all edges orded all original edges followd by inverted edges
*/
struct flow_graph
{
    int v;
    int e;
    int s;
    int t;
    int f;
    struct edge** neighbor_table; //allocate x+y list heads
    struct edge* edges;
};


/**
 * Structure for edges in flow graph
 * 
 * from node edge goes from
 * to node edge goes to
 * capacity capacity of edge
 * inverse pointer to the edge with inverted from and to
 * tail next edge in linked list with edges that have the same from node
 */
struct edge
{
    int from;
    int to;
    int capacity;
    struct edge* inverse;
    struct edge* next;
};

/* Graph functions */
struct flow_graph* initFlowGraph(int v, int e, int s, int t);
void initEdge(struct flow_graph* graph, struct edge* edges, struct edge* invedges, int from, int to, int capacity);
int BFS(struct flow_graph* cf, struct edge** visited);
void findLenAndMinC(struct edge** visited, int* lenAminC, int s, int t);
void reconstructPath(struct edge** visited, struct edge** path, int len, int last);

#endif
