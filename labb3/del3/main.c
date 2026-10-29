/**
 * Exempel på in- och utdatahantering för maxflödeslabben i kursen
 * ADK.
 *
 * Använder scanf/printf i C.
 *
 * Author: Per Austrin
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "graphutil.h"

struct graph *readBipartiteGraph() {
  int x, y, e;

  // Läs antal hörn och kanter
  scanf("%d%d%d", &x, &y, &e);

  int *edges = malloc(sizeof(int) * 2 * e);
  struct graph *graph = malloc(sizeof(struct graph));
  *graph = (struct graph){x, y, e, edges};

  // Läs in kanterna
  for (int i = 0; i < 2 * e - 1; i = i + 2) {
    int a, b;
    scanf("%d %d", &a, &b);
    graph->edges[i] = a;
    graph->edges[i + 1] = b;
  }

  return graph;
}

struct flow_graph* writeFlowGraph(struct graph *graph) {
  int v = (graph->x) + (graph->y) + 2, e = graph->e + v - 2, s = v - 1, t = v;

  // Skriv ut antal hörn och kanter samt källa och sänka
  struct flow_graph* fgraph = init_flow_graph(v,e,s,t);
  struct neighbor_list* edge = fgraph->neighbors;
  struct neighbor_list* invedge = fgraph->neighbors+e;

  for (int i = 0; i < graph->e * 2 - 1; i = i + 2) {
    init_add_edge(fgraph, edge, invedge, graph->edges[i], graph->edges[i + 1], 1);
    edge++;
    invedge++;
  }

  for (int i = 1; i <= graph->x; i++) {
    init_add_edge(fgraph, edge, invedge, s, i, 1);
    edge++;
    invedge++;
  }

  for (int i = graph->x + 1; i <= graph->y + graph->x; i++) {
    init_add_edge(fgraph, edge, invedge, i, t, 1);
    edge++;
    invedge++;
  }

  return fgraph;
}


//forwards for cf, backwards for f and cf+f for c TODO: Implement
//forwards cf so that BFS works (want to iterate search on cf)
void findMaxFlowGraph(struct flow_graph* cgraph){
  struct neighbor_list** searchTree = calloc(cgraph->v, sizeof(struct neighbor_list*));

  while(1){
    if (BFS(cgraph, searchTree) == 0){ //done when no path to increse flow
      free(searchTree);
      return;
    }

    int* lenAminC = malloc(sizeof(int)*2);
    findLenAndMinC(searchTree,lenAminC, cgraph->s, cgraph->t);

    int len = lenAminC[0];
    int minC = lenAminC[1];

    free(lenAminC); 

    struct neighbor_list** path = malloc(sizeof(struct neighbor_list*)*len);
    reconstructPath(searchTree, path, len, cgraph->t);

    for(int i = 0 ; i < len ; i++){
      path[i]->capacity-=minC;
      path[i]->inverse->capacity+=minC;
    }

    free(path);
    cgraph->f += minC;

    memset(searchTree,0,cgraph->v*sizeof(struct neighbor_list*));
  }
}

struct flow_graph* writeMaxFlowSolution(struct flow_graph* flowgraph){
  struct neighbor_list* edges = flowgraph->neighbors; 
  int e = 0;

  for (int i = 0; i < flowgraph->e; i++){
    if((edges+i)->inverse->capacity > 0){
        e++;
    }
  }

  struct flow_graph* fgraph = init_flow_graph(flowgraph->v,e,flowgraph->s,flowgraph -> t);
  fgraph->f = flowgraph->f;
  struct neighbor_list* edge = fgraph->neighbors;
  struct neighbor_list* invedge = fgraph->neighbors+e;

  //iterates over forward edges (not inverses)
  for (int i = 0; i < flowgraph->e; i++){
    if((edges+i)->inverse->capacity > 0){
    init_add_edge(fgraph, edge, invedge, (edges+i)->node, (edges+i)->neighbor, (edges+i)->inverse->capacity);
    edge++;
    invedge++;
    }
  }

  return fgraph;
}

struct graph *readMaxFlowSolution(struct flow_graph* maxgraph, int x, int y) {
  int v = maxgraph->v, e = maxgraph->e, s = maxgraph->s, t = maxgraph->t, f = maxgraph->f;


  int *edges = malloc(sizeof(int) * 2 * e);
  int edgecount = 0;

  struct graph *fgraph = malloc(sizeof(struct graph));
  *fgraph = (struct graph){x, y, edgecount, edges};

  for (int i = 0; i < e; i++) {
    int u = maxgraph->neighbors[i].node, v = maxgraph->neighbors[i].neighbor, fl = maxgraph->neighbors[i].capacity;
    // Flöde f från u till v
    if (fl > 0 && u != s && v != t) {
      fgraph->edges[edgecount] = u;
      fgraph->edges[edgecount + 1] = v;
      edgecount = edgecount + 2;
    }
  }

  fgraph->e = (edgecount) / 2;

  return fgraph;
}

void writeBipMatchSolution(struct graph *fgraph) {

  // Skriv ut antal hörn och storleken på matchningen
  printf("%d %d\n%d\n", fgraph->x, fgraph->y, fgraph->e);

  for (int i = 0; i < fgraph->e * 2 - 1; i = i + 2) {
    // Kant mellan a och b ingår i vår matchningslösning
    printf("%d %d\n", fgraph->edges[i], fgraph->edges[i + 1]);
  }
  fflush(stdout);
}

void destroy_fgraph(struct flow_graph* graph){
    free(graph->neighbor_table);
    free(graph->neighbors);
    free(graph);
}

void destroy_graph(struct graph *graph) {
  free(graph->edges);
  free(graph);
}

int main(void) {
  struct graph *graph = readBipartiteGraph();

  struct flow_graph* fgraph = writeFlowGraph(graph);

  //struct flow_graph* fgraph = readFlowGraph();

  findMaxFlowGraph(fgraph);

  struct flow_graph* maxgraph = writeMaxFlowSolution(fgraph);
    
  struct graph* mgraph = readMaxFlowSolution(maxgraph, graph->x, graph->y);

  writeBipMatchSolution(mgraph);
    
  destroy_fgraph(fgraph);
  destroy_fgraph(maxgraph);
  destroy_graph(graph);
  destroy_graph(mgraph);

  return 0;
}
