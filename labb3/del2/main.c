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


struct flow_graph* readFlowGraph(){
  int v, s, t, e;

  scanf("%d\n%d %d\n%d\n", &v, &s, &t, &e);
  
  struct flow_graph* fgraph = init_flow_graph(v,e,s,t);
  struct neighbor_list* edge = fgraph->neighbors;
  struct neighbor_list* invedge = fgraph->neighbors+e;

  for(int i = 0; i < e; i++){
    int u,v,c;
    scanf("%d%d%d", &u, &v, &c);
    init_add_edge(fgraph, edge, invedge, u, v, c);
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

void writeMaxFlowSolution(struct flow_graph* flowgraph){
  printf("%d\n%d %d %d\n%d\n", flowgraph->v, flowgraph->s,flowgraph->t,flowgraph->f, flowgraph->e);

  struct neighbor_list* edges = flowgraph->neighbors; 

  //iterates over forward edges (not inverses)
  for (int i = 0; i < flowgraph->e; i++){
    printf("%d %d %d\n",(edges+i)->node,(edges+i)->neighbor,(edges+i)->inverse->capacity);
  }
  fflush(stdout);
}


struct flow_graph* MaxFlowGraph(struct flow_graph* fgraph);

int main(void) {
  struct flow_graph* fgraph = readFlowGraph();

  findMaxFlowGraph(fgraph);

  writeMaxFlowSolution(fgraph);

  destroy_graph(fgraph);

  // debugutskrift
  //fprintf(stderr, "Bipred avslutar\n");
  return 0;
}
