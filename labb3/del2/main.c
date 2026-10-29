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
  while(1){
    struct neighbor_list** tree = BFS(cgraph);

    if (tree == NULL){ //done when no path to increse flow
      return;
    }

    printf("i reached 1\n");
    int* lenAminC = malloc(sizeof(int)*2);
    findLenAndMinC(tree,lenAminC, cgraph->s, cgraph->t);
    printf("i reached 2\n");

    int len = lenAminC[0];
    int minC = lenAminC[1];

    free(lenAminC); 

    printf("i reached 3\n");
    struct neighbor_list** path = reconstructPath(tree, len, cgraph->t);
    printf("i reached 4\n");

    for(int i = 0 ; i < len ; i++){
      path[i]->capacity-=minC;
      path[i]->inverse->capacity+=minC;
    }
    cgraph->f += minC;
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
