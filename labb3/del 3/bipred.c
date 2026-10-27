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
  for (int i = 0; i < e; ++i) {
    int a, b;
    scanf("%d %d", &a, &b);
    add_edge(graph,a,b);
  }
}


void writeFlowGraph(struct bi_graph* graph) {
  int v = (graph->x)+(graph->y)+2, e = graph->e, s=v-1, t=v;

  // Skriv ut antal hörn och kanter samt källa och sänka
  printf("%d\n%d %d\n%d\n", v, s, t, e);
  
  for (int i = 0; i < e; ++i) {
    int u, v, c;
    // Kant från u till v med kapacitet c
    printf("%d %d %d\n", u, v, c);
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

  // debugutskrift
  fprintf(stderr, "Bipred avslutar\n");
  return 0;
}
