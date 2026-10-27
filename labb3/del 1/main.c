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

  struct bi_graph* graph = init_bi_graph(x,y,e);

  // Läs in kanterna
  struct neighbor_list* edges = (graph->neighbors);
  struct neighbor_list* invedges = (graph->neighbors)+e;
  for (int i = 0; i < e; i++) {
    int a, b;
    scanf("%d %d", &a, &b);
    init_add_edge(graph,edges,invedges,a,b);
    edges++;
    invedges++;
  }
  
  return graph;
}

void writeFlowGraph(struct bi_graph* graph) {
  int v = (graph->x)+(graph->y)+2, e = graph->e + v - 2, s=v-1, t=v;

  // Skriv ut antal hörn och kanter samt källa och sänka
  printf("%d\n%d %d\n%d\n", v, s, t, e);
  
  for (int i = 0 ; i < graph->x ; i++){
    struct neighbor_list* x_neighbors = graph->neighbor_table[i];
    while(x_neighbors->tail != NULL){
      printf("%d %d %d\n",i+1,x_neighbors->neighbor,1);
      x_neighbors = x_neighbors->tail;
    }
    printf("%d %d %d\n",i+1,x_neighbors->neighbor,1);
  }
  for (int i = graph->x + 1 ; i <= (graph->x + graph->y); i++){
    printf("%d %d %d\n",i,t,1);
  }
  for (int i = 1 ; i <= graph->x ; i++){
    printf("%d %d %d\n",s,i,1);
  }

  // Var noggrann med att flusha utdata när flödesgrafen skrivits ut!
  fflush(stdout);

  // Debugutskrift
  //fprintf(stderr, "Skickade iväg flödesgrafen\n");
}

struct bi_graph* readMaxFlowSolution() {
  int v, e, s, t, f;

  // Läs in antal hörn, kanter, källa, sänka, och totalt flöde
  // (Antal hörn, källa och sänka borde vara samma som vi i grafen vi
  // skickade iväg)
  scanf("%d%d%d%d%d", &v, &s, &t, &f, &e);

  struct bi_graph* fgraph = init_bi_graph(v-2,0,e-s-1);

  int counter = 0;

  struct neighbor_list* edges = (fgraph->neighbors);
  struct neighbor_list* invedges = (fgraph->neighbors)+e;
  for (int i = 0; i < e; ++i) {
    int u, v, f;
    scanf("%d%d%d", &u, &v, &f);
    if (u == s){
      counter++;
    } else if (v != t) {
      init_add_edge_c(fgraph,edges,invedges,u,v,f);
      edges++;
      invedges++;
    }
    
    fgraph->y = fgraph->x - counter;
    fgraph->x = counter;
    // Flöde f från u till v
  }
}


void writeBipMatchSolution(struct bi_graph* fgraph) {
  int x = 17, y = 4711, maxMatch = 0;

  // Skriv ut antal hörn och storleken på matchningen
  printf("%d %d\n%d\n", x, y, maxMatch);

  for (int i = 0; i < maxMatch; ++i) {
    int a, b;
    // Kant mellan a och b ingår i vår matchningslösning
    printf("%d %d\n", a, b);
  }

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
