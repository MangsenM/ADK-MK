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


void readBipartiteGraph(struct bi_graph* graph) {
  int x, y, e, i;
  // Läs antal hörn och kanter
  scanf("%d%d%d", &x, &y, &e);

  graph = init_bi_graph(x,y,e);

  // Läs in kanterna
  struct neighbor_list* forwards = &(graph->neighbor_table)[x+y];
  struct neighbor_list* backwards = &(graph->neighbor_table)[x+y+e];
  for (int i = 0; i < e; ++i) {
    int a, b;
    scanf("%d %d", &a, &b);
    init_add_edge(graph,forwards,backwards,a,b);
    forwards++;
    backwards++;
  }
}

void writeFlowGraph(struct bi_graph* graph) {
  int v = (graph->x)+(graph->y)+2, e = graph->e, s=v-1, t=v;

  // Skriv ut antal hörn och kanter samt källa och sänka
  printf("%d\n%d %d\n%d\n", v, s, t, e);
  
  for (int i = 1 ; i < graph->x ; i++){
    printf("%d%d%d",s,i,1);
  }
  for (int i = 1 ; i < (graph->y - graph->x) ; i++){
    printf("%d%d%d",i,t,1);
  }
  for (int i = 0 ; i < graph->x ; i++){
    struct neighbor_list x_neighbors = graph->neighbor_table[i];
    while(x_neighbors.neighbor){
      printf("%d%d%d",i+1,x_neighbors.neighbor,1);
      x_neighbors = *x_neighbors.tail;
    }
  }

  // Var noggrann med att flusha utdata när flödesgrafen skrivits ut!
  fflush(stdout);

  // Debugutskrift
  fprintf(stderr, "Skickade iväg flödesgrafen\n");
}


int main(void) {
  struct bi_graph* graph;

  readBipartiteGraph(graph);

  writeFlowGraph(graph);

  destroy_graph(graph);

  // debugutskrift
  fprintf(stderr, "Bipred avslutar\n");
  return 0;
}
