/**
 * Exempel på in- och utdatahantering för maxflödeslabben i kursen
 * ADK.
 *
 * Använder scanf/printf i C.
 *
 * Author: Per Austrin
 */

#include <stdio.h>
#include "graphutil.h"


struct bi_graph* readFlowGraph(){
  int v, s, t, e;

  scanf("%d\n%d %d\n%d\n", v, s, t, e);
} 

int main(void) {
  struct bi_graph* graph = readBipartiteGraph();

  writeFlowGraph(graph);

  destroy_graph(graph);

  struct bi_graph* flowgraph = readMaxFlowSolution();

  writeBipMatchSolution(flowgraph);
  // debugutskrift
  //fprintf(stderr, "Bipred avslutar\n");
  return 0;
}
