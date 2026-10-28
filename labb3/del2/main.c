/**
 * Exempel på in- och utdatahantering för maxflödeslabben i kursen
 * ADK.
 *
 * Använder scanf/printf i C.
 *
 * Author: Per Austrin
 */

#include <stdio.h>
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
    printf("%d %d %d\n", u, fgraph->neighbors[i].neighbor, fgraph->neighbors[i].capacity);
    fflush(stdout);
  }

  return fgraph;
} 

struct flow_graph* findMaxFlowGraph(struct flow_graph* fgraph);

void writeMaxFlowSolution(struct flow_graph* flowgraph){
  printf("%d\n%d %d\n%d\n", flowgraph->v, flowgraph->s,flowgraph->t,flowgraph->e);
  
  for (int i = 0; i < flowgraph->v; i++){
    struct neighbor_list* fgnlist = flowgraph->neighbor_table[i];
    int check = 1;
    if(fgnlist->tail == NULL){
        check = 0;
    }
    while(check == 1){
        printf("%d %d %d\n", i + 1, fgnlist->neighbor, fgnlist->capacity);
        sleep(1);
    if(fgnlist->tail == NULL){
        check = 0;
    } else {
        fgnlist = fgnlist->tail;
    }
    }
  }
    fflush(stdout);
}

struct flow_graph* MaxFlowGraph(struct flow_graph* fgraph);

int main(void) {
  struct flow_graph* fgraph = readFlowGraph();

  //struct flow_graph* maxfgraph = findMaxFlowGraph(fgraph);

  writeMaxFlowSolution(fgraph);

  destroy_graph(fgraph);
  //destroy_graph(maxfgraph);


  // debugutskrift
  //fprintf(stderr, "Bipred avslutar\n");
  return 0;
}
