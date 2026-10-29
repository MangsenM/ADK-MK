/**
 * Exempel på in- och utdatahantering för maxflödeslabben i kursen
 * ADK.
 *
 * Använder scanf/printf i C.
 *
 * Author: Per Austrin, Karmela Eliasson, Malcolm Mangsen
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "graphutil.h"

/**
 * Create bipartite graph from stdin
 */
struct bi_graph *readBipartiteGraph() {
  int x, y, e;

  // Read: vertices in x, vertices in y, edges
  scanf("%d%d%d", &x, &y, &e);

  int *edges = malloc(sizeof(int) * 2 * e); //double as each edge takes two spaces
  struct bi_graph *graph = malloc(sizeof(struct bi_graph));
  *graph = (struct bi_graph){x, y, e, edges};

  // Read edges one by one, move i+2 as edges take two spaces each
  for (int i = 0; i < 2 * e - 1; i = i + 2) {
    int from, to;
    scanf("%d %d", &from, &to);
    graph->edges[i] = from; 
    graph->edges[i + 1] = to;
  }

  return graph;
}

/**
 * Translate bipartite graph to flow graph
 * 
 * @param graph bipartite graph
 * @return the resulting flow graph
 */
struct flow_graph* writeFlowGraph(struct bi_graph *graph) {
  int v = (graph->x) + (graph->y) + 2; //2 extra for s and t
  int e = graph->e + v - 2; //v-2 for edges for s to x and from y to t
  int s = v - 1;
  int t = v;

  struct flow_graph* f_graph = initFlowGraph(v,e,s,t);
  struct edge* edge = f_graph->edges;
  struct edge* invedge = f_graph->edges+e;

  for (int i = 0; i < graph->e * 2 - 1; i = i + 2) {
    initEdge(f_graph, edge, invedge, graph->edges[i], graph->edges[i + 1], 1);
    edge++;
    invedge++;
  }

  //edges from source to nodes in x
  for (int i = 1; i <= graph->x; i++) {
    initEdge(f_graph, edge, invedge, s, i, 1);
    edge++;
    invedge++;
  }

  //edges from nodes in y to target
  for (int i = graph->x + 1; i <= graph->y + graph->x; i++) {
    initEdge(f_graph, edge, invedge, i, t, 1);
    edge++;
    invedge++;
  }

  return f_graph;
}

/**
 * Finds max flow in initialized flow graph, written to f member in graph
 * 
 * "Forward edges" = cf, "Backward edges" (inverse) = f and cf+f = c
 * 
 * @param graph graph to find max flow in
 */
void findMaxFlowGraph(struct flow_graph* graph){
  struct edge** searchTree = calloc(graph->v, sizeof(struct edge*));

  while(1){
    if (BFS(graph, searchTree) == 0){ //done when no path to increase flow
      free(searchTree);
      return;
    }

    int* lenAminC = malloc(sizeof(int)*2);
    findLenAndMinC(searchTree,lenAminC, graph->s, graph->t);

    int len = lenAminC[0];
    int minC = lenAminC[1];

    free(lenAminC); 

    struct edge** path = malloc(sizeof(struct edge*)*len);
    reconstructPath(searchTree, path, len, graph->t);

    for(int i = 0 ; i < len ; i++){
      path[i]->capacity-=minC; //update cf
      path[i]->inverse->capacity+=minC; //update f
    }

    free(path);
    graph->f += minC; //update maxflow

    //reset searchtree for next BFS (used as visited array)
    memset(searchTree,0,graph->v*sizeof(struct edge*));
  }
}

/**
 * Creates final flow graph from compound flow and residual graph
 * 
 * @param c_graph compound flow and residual graph
 * @return flow graph only containing final max flow in graph
 */
struct flow_graph* writeMaxFlowSolution(struct flow_graph* c_graph){
  struct edge* edges = c_graph->edges; 
  int e = 0;

  //remove edges not used in max flow from total number of edges
  for (int i = 0; i < c_graph->e; i++){
    if((edges+i)->inverse->capacity > 0){
        e++;
    }
  }

  struct flow_graph* f_graph = initFlowGraph(c_graph->v,e,c_graph->s,c_graph -> t);
  f_graph->f = c_graph->f;
  struct edge* edge = f_graph->edges;
  struct edge* invedge = f_graph->edges+e;

  //iterates over forward edges (not inverses), but take inverse capacity (final flow)
  for (int i = 0; i < c_graph->e; i++){
    if((edges+i)->inverse->capacity > 0){
    initEdge(f_graph, edge, invedge, (edges+i)->from, (edges+i)->to, (edges+i)->inverse->capacity);
    edge++;
    invedge++;
    }
  }

  return f_graph;
}

/**
 * Converts max flow graph to bipartite graph
 * 
 * @param max_graph calculated max flow graph
 * @param x vertseries in x bipartite graph
 * @param y vertseries in y bipartite graph
 */
struct bi_graph *readMaxFlowSolution(struct flow_graph* max_graph, int x, int y) {
  int v = max_graph->v, e = max_graph->e, s = max_graph->s, t = max_graph->t, f = max_graph->f;


  int *edges = malloc(sizeof(int) * 2 * e);
  int edgecount = 0;

  struct bi_graph *b_graph = malloc(sizeof(struct bi_graph));
  *b_graph = (struct bi_graph){x, y, edgecount, edges};

  for (int i = 0; i < e; i++) {
    int u = max_graph->edges[i].from, v = max_graph->edges[i].to, fl = max_graph->edges[i].capacity;
    // flow f from u to v
    if (fl > 0 && u != s && v != t) {
      b_graph->edges[edgecount] = u;
      b_graph->edges[edgecount + 1] = v;
      edgecount = edgecount + 2;
    }
  }

  b_graph->e = (edgecount) / 2;

  return b_graph;
}

void writeBipMatchSolution(struct bi_graph *b_graph) {

  // Write number of vertseries in x, y and the number of edges
  printf("%d %d\n%d\n", b_graph->x, b_graph->y, b_graph->e);

  for (int i = 0; i < b_graph->e * 2 - 1; i = i + 2) {
    // Edges from max flow are the same as edges in match solution
    printf("%d %d\n", b_graph->edges[i], b_graph->edges[i + 1]);
  }
  fflush(stdout);
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

/**
 * Deallocates all structures in bipartite graph
 * 
 * @param graph To destroy
 */
void destroy_bi_graph(struct bi_graph *graph) {
  free(graph->edges);
  free(graph);
}

int main(void) {
  struct bi_graph *graph = readBipartiteGraph();

  struct flow_graph* f_graph = writeFlowGraph(graph);

  findMaxFlowGraph(f_graph);

  struct flow_graph* max_graph = writeMaxFlowSolution(f_graph);
    
  struct bi_graph* match_graph = readMaxFlowSolution(max_graph, graph->x, graph->y);

  writeBipMatchSolution(match_graph);
    
  destroy_flow_graph(f_graph);
  destroy_flow_graph(max_graph);
  destroy_bi_graph(graph);
  destroy_bi_graph(match_graph);

  return 0;
}
