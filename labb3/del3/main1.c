/**
 * Exempel på in- och utdatahantering för maxflödeslabben i kursen
 * ADK.
 *
 * Modifierad för att lösa deluppgift 1.
 *
 * Använder scanf/printf i C.
 *
 * Author: Per Austrin & Malcolm Mångsén & Karmela Eliasson
 */

#include <stdio.h>
#include <stdlib.h>

struct graph {
  int x;
  int y;
  int e;
  int *edges;
};

struct graph *readBipartiteGraph() {
  int x, y, e;

  // Läs antal hörn och kanter
  scanf("%d%d%d", &x, &y, &e);

  int *edges = malloc(sizeof(int) * 2 * e);
  struct graph *graph = malloc(sizeof(struct graph));
  *graph = (struct graph){x, y, e, edges};

  // Läs in kanterna
  for (int i = 0; i < 2 * e - 1; i = i + 2) {
    int a, b;
    scanf("%d %d", &a, &b);
    graph->edges[i] = a;
    graph->edges[i + 1] = b;
  }

  return graph;
}

void writeFlowGraph(struct graph *graph) {
  int v = (graph->x) + (graph->y) + 2, e = graph->e + v - 2, s = v - 1, t = v;

  // Skriv ut antal hörn och kanter samt källa och sänka
  printf("%d\n%d %d\n%d\n", v, s, t, e);

  for (int i = 0; i < graph->e * 2 - 1; i = i + 2) {
    printf("%d %d %d\n", graph->edges[i], graph->edges[i + 1], 1);
  }

  for (int i = 1; i <= graph->x; i++) {
    printf("%d %d %d\n", s, i, 1);
  }

  for (int i = graph->x + 1; i <= graph->y + graph->x; i++) {
    printf("%d %d %d\n", i, t, 1);
  }

  // Var noggrann med att flusha utdata när flödesgrafen skrivits ut!
  fflush(stdout);

  // Debugutskrift
  // fprintf(stderr, "Skickade iväg flödesgrafen\n");
}

struct graph *readMaxFlowSolution(int x, int y) {
  int v, e, s, t, f;

  // Läs in antal hörn, kanter, källa, sänka, och totalt flöde
  // (Antal hörn, källa och sänka borde vara samma som vi i grafen vi
  // skickade iväg)
  scanf("%d%d%d%d%d", &v, &s, &t, &f, &e);

  int *edges = malloc(sizeof(int) * 2 * e);
  int edgecount = 0;

  struct graph *fgraph = malloc(sizeof(struct graph));
  *fgraph = (struct graph){x, y, edgecount, edges};

  for (int i = 0; i < e; i++) {
    int u, v, fl;
    // Flöde f från u till v
    scanf("%d%d%d", &u, &v, &fl);
    if (fl > 0 && u != s && v != t) {
      fgraph->edges[edgecount] = u;
      fgraph->edges[edgecount + 1] = v;
      edgecount = edgecount + 2;
    }
  }

  fgraph->e = (edgecount) / 2;

  return fgraph;
}

void writeBipMatchSolution(struct graph *fgraph) {

  // Skriv ut antal hörn och storleken på matchningen
  printf("%d %d\n%d\n", fgraph->x, fgraph->y, fgraph->e);

  for (int i = 0; i < fgraph->e * 2 - 1; i = i + 2) {
    // Kant mellan a och b ingår i vår matchningslösning
    printf("%d %d\n", fgraph->edges[i], fgraph->edges[i + 1]);
  }
  fflush(stdout);
}

void destroy_graph(struct graph *graph) {
  free(graph->edges);
  free(graph);
}

int main(void) {
  struct graph *graph = readBipartiteGraph();

  writeFlowGraph(graph);

  struct graph *fgraph = readMaxFlowSolution(graph->x, graph->y);

  writeBipMatchSolution(fgraph);

  destroy_graph(graph);
  destroy_graph(fgraph);
  // debugutskrift
  // fprintf(stderr, "Bipred avslutar\n");
  return 0;
}
