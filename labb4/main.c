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
        graph->nb_table[to - 1][from - 1] = (struct edge){to, from};
        graph->val[from - 1]++;
    }
    
    for (int i = 0; i < v; i ++){
        graph->max = graph->max < graph->val[i] ? graph->val[i] : graph->max;
    }

    //rmIsolatedNodes(graph);

    return graph;
}

void color2cast(struct graph* graph){
    //to do spex case m = 1
    if (graph->m == 1 && graph->e == 0 || graph->m > graph->max){
        printf ("3\n2\n3\n1 1\n1 2\n1 3\n2 1 3\n2 2 3\n");
        return;

    } else if (graph->m == 1 && graph->e > 0){
        printf ("1\n1\n2\n2 1 2\n1 1\n");
        return;
    }

    
    //n, s, k values.
    printf("%d\n%d\n%d\n", graph->v + 3, graph->s+2, graph->m + 3);
    
    //divas
    printf("1 1\n");
    printf("1 2\n");

    //role restrictions
    for(int i = 0; i < graph->v; i++){

        printf("%d", graph->m);

        for (int j = 1; j <= graph->m; j++){
            printf(" %d", j + 2);
        }

        printf("\n");
    }
    
    //bonus nodes
    printf("1 %d\n", graph->m + 3);

    //scenes

    for (int i = 0; i < graph->v; i++){
        
        if (graph->nb_table[i] != NULL){
            printf("%d %d", graph->val[i], i + 2);
        for (int j = 0; i < graph->e; i++){
            if (graph->nb_table[i][j].to != 0){
                printf(" %d",graph->nb_table[i][j].to);
            }
        }
            printf("\n");
        }

    }

    //divas scenes
    printf("2 %d 1\n", graph->v + 3);
    printf("2 %d 2\n", graph->v + 3);

    fflush(stdout);
}

void destroyGraph(struct graph* graph){
    for(int i = 0; i < graph->v; i++){
    free(graph->nb_table[i]);
    }
    free(graph->val);
    free(graph);
}

int main(void){

    struct graph* graph = readColorInstance();

    color2cast(graph);

    destroyGraph(graph);

    return 0;
}
