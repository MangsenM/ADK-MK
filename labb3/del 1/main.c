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


struct bi_graph* readBipartiteGraph() {
  int x, y, e;
  // Läs antal hörn och kanter
  scanf("%d%d%d", &x, &y, &e);
    
  printf("read graf with values:\n");
  printf("x = %d\ny = %d\ne = %d\n",x,y,e);

  printf("about to init graph\n");

  struct bi_graph* graph = init_bi_graph(x,y,e);

  printf("graph initialized with:\n");
  printf("x = %d\ny = %d\ne = %d\n",graph->x,graph->y,graph->e);
  // Läs in kanterna
  struct neighbor_list* edges = (graph->neighbors);
  struct neighbor_list* invedges = (graph->neighbors)+e;
  for (int i = 0; i < e; i++) {
  printf("graph has values with during loop:\n");
  printf("x = %d\ny = %d\ne = %d\n",graph->x,graph->y,graph->e);
    int a, b;
    scanf("%d %d", &a, &b);
    printf("about to init edges\n");
    init_add_edge(graph,edges,invedges,a,b);
    printf("edges initialized\n");
    edges++;
    invedges++;
  }
  
  return graph;
}

void writeFlowGraph(struct bi_graph* graph) {
  int v = (graph->x)+(graph->y)+2, e = graph->e, s=v-1, t=v;
  printf("entered flowgraph\n");
  printf("graph has values with:\n");
  printf("x = %d\ny = %d\ne = %d\n",graph->x,graph->y,graph->e);

  // Skriv ut antal hörn och kanter samt källa och sänka
  printf("%d\n%d %d\n%d\n", v, s, t, e);
  
  for (int i = 1 ; i <= graph->x ; i++){
    printf("%d %d %d\n",s,i,1);
  }
  for (int i = graph->x + 1 ; i <= (graph->x + graph->y); i++){
    printf("%d %d %d\n",i,t,1);
  }
  for (int i = 0 ; i < graph->x ; i++){
    printf("starting flowgraph loop, for x nodes \n");
    struct neighbor_list* x_neighbors = graph->neighbor_table[i];
    printf("entering while loop\n");
    while(x_neighbors->tail != NULL){
    printf("entered while loop\n");
      printf("%d %d %d\n",i+1,x_neighbors->neighbor,1);
      x_neighbors = x_neighbors->tail;
    }
    printf("%d %d %d\n",i+1,x_neighbors->neighbor,1);
  }

  // Var noggrann med att flusha utdata när flödesgrafen skrivits ut!
  fflush(stdout);

  // Debugutskrift
  fprintf(stderr, "Skickade iväg flödesgrafen\n");
}


int main(void) {
  struct bi_graph* graph = readBipartiteGraph();
  printf("graph has values with in main:\n");
  printf("x = %d\ny = %d\ne = %d\n",graph->x,graph->y,graph->e);

  writeFlowGraph(graph);

  destroy_graph(graph);

  // debugutskrift
  fprintf(stderr, "Bipred avslutar\n");
  return 0;
}
