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

struct bi_graph {
  int x;
  int y;
  int e;
  int *edges;
};

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
 */
void writeFlowGraph(struct bi_graph *graph) {
  int v = (graph->x) + (graph->y) + 2; //2 extra for s and t
  int e = graph->e + v - 2; //v-2 for edges for s to x and from y to t
  int s = v - 1;
  int t = v;

  // Skriv ut antal hörn och kanter samt källa och sänka
  printf("%d\n%d %d\n%d\n", v, s, t, e);

  for (int i = 0; i < graph->e * 2 - 1; i = i + 2) {
    printf("%d %d %d\n", graph->edges[i], graph->edges[i + 1], 1);
  }

  //edges from source to nodes in x
  for (int i = 1; i <= graph->x; i++) {
    printf("%d %d %d\n", s, i, 1);
  }

  //edges from nodes in y to target
  for (int i = graph->x + 1; i <= graph->y + graph->x; i++) {
    printf("%d %d %d\n", i, t, 1);
  }

  fflush(stdout);
}

/**
 * Converts max flow graph to bipartite graph
 * 
 * @param x vertseries in x bipartite graph
 * @param y vertseries in y bipartite graph
 */
struct bi_graph *readMaxFlowSolution(int x, int y) {
  int v, e, s, t, f;

  scanf("%d%d%d%d%d", &v, &s, &t, &f, &e);

  int *edges = malloc(sizeof(int) * 2 * e);
  int edgecount = 0;

  struct bi_graph *f_graph = malloc(sizeof(struct bi_graph));
  *f_graph = (struct bi_graph){x, y, edgecount, edges};

  for (int i = 0; i < e; i++) {
    int u, v, fl;
    // flow f from u to v
    scanf("%d%d%d", &u, &v, &fl);
    if (fl > 0 && u != s && v != t) {
      f_graph->edges[edgecount] = u;
      f_graph->edges[edgecount + 1] = v;
      edgecount = edgecount + 2;
    }
  }

  f_graph->e = (edgecount) / 2;

  return f_graph;
}

/**
 * Print bipartite match solution
 */
void writeBipMatchSolution(struct bi_graph *f_graph) {

  // Write number of vertseries in x, y and the number of edges
  printf("%d %d\n%d\n", f_graph->x, f_graph->y, f_graph->e);

  for (int i = 0; i < f_graph->e * 2 - 1; i = i + 2) {
    // Edges from max flow are the same as edges in match solution
    printf("%d %d\n", f_graph->edges[i], f_graph->edges[i + 1]);
  }
  fflush(stdout);
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

  writeFlowGraph(graph);

  struct bi_graph *f_graph = readMaxFlowSolution(graph->x, graph->y);

  writeBipMatchSolution(f_graph);

  destroy_bi_graph(graph);
  destroy_bi_graph(f_graph);
  return 0;
}
