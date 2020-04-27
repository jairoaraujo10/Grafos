#include "libraries/grafos.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int V = 8, i, j;
    vertex src = 0;
    Graph *G = createGraph(V);

    addEdge(G, 0, 1, 1);
    addEdge(G, 0, 2, 3);
    addEdge(G, 0, 4, 6);
    addEdge(G, 1, 2, 1);
    addEdge(G, 2, 3, -5);
    addEdge(G, 3, 5, 7);
    addEdge(G, 4, 3, 6);
    addEdge(G, 5, 6, 3);
    addEdge(G, 5, 4, 1);
    addEdge(G, 6, 7, -2);
    addEdge(G, 7, 5, -1);
    addEdge(G, 4, 7, 1);

    showAdjList(G);
    showAdjMatrix(G);

    printf("Algoritmo de Bellman-Ford:\n");
    CaminhosMinimos *C0 = BellmanFord(G, src);
    imprimeCaminhos(C0);
    liberaCaminhos(C0);

    printf("Algoritmo de Dijkstra:\n");
    CaminhosMinimos *C1 = Dijkstra(G, src);
    imprimeCaminhos(C1);
    liberaCaminhos(C1);

    printf("Algoritmo de Johnson:\n");
    CaminhosMinimos **CMin = Johnson(G);
    for (i = 0; i < G->V; i++) {
        imprimeCaminhos(CMin[i]);
    }

    // Imprime a matriz dos menores caminhos entre todos os pares de vértices
    for (i = 0; i < sizeof(CMin); i++) {
        for (j = 0; j < G->V; j++) {
            if (CMin[i]->array[j].weight == infn) printf("\tinfn");
            else printf("\t%.2f", CMin[i]->array[j].weight);
        }
        printf("\n");
    }

    for (i = 0; i < G->V; i++) {
        liberaCaminhos(CMin[i]);
    }
    free(CMin);
    liberaGrafo(G);
    return 1;
}