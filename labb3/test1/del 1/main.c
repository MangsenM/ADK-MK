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
//#include "graphutil.h"

struct graph
{
    int x;
    int y;
    int e;
    int* edges;
};




struct graph* readBipartiteGraph() {
  int x, y, e;
  // Läs antal hörn och kanter
  scanf("%d%d%d", &x, &y, &e);

  int* edges = malloc(sizeof(int)*2*e);
  struct graph* graph = malloc(sizeof(struct graph));
  *graph = (struct graph){x,y,e,edges};

  // Läs in kanterna
  for (int i = 0; i < 2*e - 1; i = i+2) {
    int a, b;
    scanf("%d %d", &a, &b);
    graph->edges[i] = a;
    graph->edges[i+1] = b;
  }
  
  return graph;
}

void writeFlowGraph(struct graph* graph) {
  int v = (graph->x)+(graph->y)+2, e = graph->e + v - 2, s=v-1, t=v;

  // Skriv ut antal hörn och kanter samt källa och sänka
  printf("%d\n%d %d\n%d\n", v, s, t, e);
  
  for (int i = 0 ; i < graph->e*2 - 1 ;i = i+2){
      printf("%d %d %d\n",graph->edges[i],graph->edges[i + 1],1);
  }

  for (int i = 1 ; i <= graph->x ;i++){
      printf("%d %d %d\n",s,i,1);
  }

  for (int i = graph->x + 1 ; i <= graph->y + graph->x ;i++){
      printf("%d %d %d\n",i,t,1);
  }

  // Var noggrann med att flusha utdata när flödesgrafen skrivits ut!
  fflush(stdout);

  // Debugutskrift
  //fprintf(stderr, "Skickade iväg flödesgrafen\n");
}

struct graph* readMaxFlowSolution() {
  int v, e, s, t, f;

  // Läs in antal hörn, kanter, källa, sänka, och totalt flöde
  // (Antal hörn, källa och sänka borde vara samma som vi i grafen vi
  // skickade iväg)
  scanf("%d%d%d%d%d", &v, &s, &t, &f, &e);

  int* medges = malloc(sizeof(int)*2*f);
  struct graph* fgraph = malloc(sizeof(struct graph));

  int edgeindex = 0;
  int xcount = 0;
  for (int i = 0; i < f; ++i) {
    int u, v, fl;
    // Flöde f från u till v
    scanf("%d%d%d", &u, &v, &f);
    if(fl == 1 && u != s && v != t){
        medges[edgeindex] = u;
        medges[edgeindex + 1] = v;
        edgeindex = edgeindex + 2;
    }
    if (u == s){xcount++;}
  }

  *fgraph = (struct graph){xcount,v - xcount - 2,f,medges};
   return fgraph;
}


void writeBipMatchSolution(struct graph* fgraph) {

  // Skriv ut antal hörn och storleken på matchningen
  printf("%d %d\n%d\n", fgraph->x, fgraph->y, fgraph->e);

  for (int i = 0; i < fgraph->e*2 - 1; i = i + 2) {
    int a = fgraph->edges[i], b = fgraph->edges[i + 1];
    // Kant mellan a och b ingår i vår matchningslösning
    printf("%d %d\n", a, b);
  }
  fflush(stdout);

}

void destroy_graph(struct graph* graph){
    free(graph->edges);
    free(graph);
}

int main(void) {
  struct graph* graph = readBipartiteGraph();

  writeFlowGraph(graph);

  struct graph* fgraph = readMaxFlowSolution();

  writeBipMatchSolution(fgraph);

  destroy_graph(graph);
  destroy_graph(fgraph);
  // debugutskrift
  //fprintf(stderr, "Bipred avslutar\n");
  return 0;
}
