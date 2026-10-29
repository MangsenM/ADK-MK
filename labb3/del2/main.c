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

/**
 * Create flow graph from stdin
 */
struct flow_graph* readFlowGraph(){
  int v, s, t, e;

  scanf("%d\n%d %d\n%d\n", &v, &s, &t, &e);
  
  struct flow_graph* f_graph = initFlowGraph(v,e,s,t);
  struct edge* edge = f_graph->edges;
  struct edge* invedge = f_graph->edges+e;

  for(int i = 0; i < e; i++){
    int u,v,c;
    scanf("%d%d%d", &u, &v, &c);
    initEdge(f_graph, edge, invedge, u, v, c);
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
void writeMaxFlowSolution(struct flow_graph* c_graph){
  struct edge* edges = c_graph->edges; 
  int e = 0;

  //remove edges not used in max flow from total number of edges
  for (int i = 0; i < c_graph->e; i++){
    if((edges+i)->inverse->capacity > 0){
        e++;
    }
  }

  printf("%d\n%d %d %d\n%d\n", c_graph->v, c_graph->s,c_graph->t,c_graph->f, e);


  //iterates over forward edges (not inverses)
  for (int i = 0; i < c_graph->e; i++){
    if((edges+i)->inverse->capacity > 0){
    printf("%d %d %d\n",(edges+i)->from,(edges+i)->to,(edges+i)->inverse->capacity);
    }
  }
  fflush(stdout);
}

int main(void) {
  struct flow_graph* f_graph = readFlowGraph();

  findMaxFlowGraph(f_graph);

  writeMaxFlowSolution(f_graph);

  destroy_flow_graph(f_graph);

  // debugutskrift
  //fprintf(stderr, "Bipred avslutar\n");
  return 0;
}
