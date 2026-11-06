#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

struct graph* readColorInstance() {
    int v, e, m;
    scanf("%d%d%d", &v, &e, &m);

    struct graph* graph = initGraph(v, e, m);

    for (int i = 0; i < e; i++){
        int from, to;
        scanf("%d%d", &from, &to);
        addEdge(graph, from, to);
    }

    return graph;
}

void color2cast(struct graph* graph){
    int max = maxVal(graph);
    int connected = nrConnectedNodes(graph);
    int s = THEEVERYTHINGFIX(graph);

    //to do spex case m = 1 and more colors than nodes
    if ((graph->m == 1 && graph->e == 0) || graph->m > max){
        printf ("3\n2\n3\n1 1\n1 2\n1 3\n2 1 3\n2 2 3\n");
        return;

    } else if (graph->m == 1 && graph->e > 0){
        printf ("1\n1\n2\n2 1 2\n1 1\n");
        return;
    }
    
    //n, s, k values.
    printf("%d\n%d\n%d\n", connected+2, s+2, graph->m+2);
    
    //divas
    printf("1 1\n");
    printf("1 2\n");

    //role restrictions
    for(int i = 0; i < connected; i++){

        printf("%d", graph->m);

        for (int j = 1; j <= graph->m; j++){
            printf(" %d", j + 2);
        }

        printf("\n");
    }

    //scenes

    int names[graph->v];
    renameConnected(graph, names);

    for (int i = 0; i < graph->v; i++){
        
        if(graph->belonging_edges[i] != 0){
            printf("%d %d", graph->belonging_edges[i]+1, names[i]);
            for(int j = i+1 ; j < graph->v ; j++){
                if(graph->nb_mrx[i][j] != 0){
                    printf(" %d",names[j]);
                }
            }
        
            printf("\n");
        }

    }

    //divas scenes
    printf("2 1 3\n");
    printf("2 2 3\n");

    fflush(stdout);
}

int main(void){

    struct graph* graph = readColorInstance();

    color2cast(graph);

    destroyGraph(graph);

    return 0;
}
