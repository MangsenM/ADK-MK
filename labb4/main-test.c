#include <stdlib.h>
#include <stdio.h>

struct graph { 
    int v;
    int e;
    int m;
    int max_d;
    int* edges;
};

void maxDegree(struct graph* graph, int* names){
    int max = 0;

    for(int i = 0 ; i < graph->v ; i++){
        if(names[i] > max){
            max = names[i];
        }
    }

    graph->max_d = max;
} 

int* readColorInstance(struct graph* graph) {
    int v, e, m;
    scanf("%d%d%d", &v, &e, &m);

    graph->v = v;
    graph->e = e;
    graph->m = m;
    graph->edges = calloc(2*e,sizeof(int));

    int* degrees = calloc(v,sizeof(int));
    int temp_visited[v][v];
    
    for (int i = 0; i < v; i++){
        for (int j = 0; j < v; j++){
            temp_visited[i][j] = 0;
        }
    }

    int i = 0;
    for (int j = 1; j <= e; j++){
        int from, to;
        scanf("%d%d", &from, &to);

        if(temp_visited[from-1][to-1] == 0 || temp_visited[to-1][from-1] == 0){
            graph->edges[i] = from;
            graph->edges[i+1] = to;

            degrees[from-1]++;
            degrees[to-1]++;
            
            temp_visited[from-1][to-1] = 1;

            i+=2;
        } else {
            graph->e--;
        }
    }

    maxDegree(graph, degrees);

    return degrees;
}

void nameNodes(struct graph* graph, int* names){
    int name = 3;

    for(int i = 0 ; i < graph->v ; i++){
        if(names[i] != 0){
            names[i] = name;
            name++;
        }
    }

    graph->v = name-3; // number of roles
}

void color2cast(struct graph* graph, int* names){

    //to do spex case m = 1 and more colors than nodes
    /*
    if ((graph->m == 1 && graph->e == 0) || graph->m > graph->max_d){
        printf ("3\n2\n3\n1 1\n1 2\n1 3\n2 1 3\n2 2 3\n");
        return;

    } else if (graph->m == 1 && graph->e > 0){
        printf ("2\n1\n2\n2 1 2 \n2 1 2 \n2 1 2\n");
        return;
    }
    */

    //n, s, k values.
    printf("%d\n%d\n%d\n", graph->v+2, graph->e+2, graph->m+2);
    
    //divas
    printf("1 1\n");
    printf("1 2\n");

    //role restrictions
    for(int i = 0; i < graph->v; i++){

        printf("%d", graph->m);

        for (int j = 3; j <= (graph->m + 2) ; j++){
            printf(" %d", j);
        }

        printf("\n");
    }

    //scenes

    for (int i = 0; i < graph->e*2-1 ; i+=2){
        printf("2 %d %d\n", names[graph->edges[i]-1], names[graph->edges[i+1]-1]);
    }

    //divas scenes
    printf("2 1 3\n");
    printf("2 2 3\n");

    fflush(stdout);
}

void destroyGraph(struct graph* graph){
    free(graph->edges);
    free(graph);
}

int main(void){

    struct graph* graph = malloc(sizeof(struct graph));
    
    int* names = readColorInstance(graph);

    nameNodes(graph, names);

    color2cast(graph, names);

    destroyGraph(graph);
    free(names);

    return 0;
}
