#include <stdio.h>
#include "grafos.h"

int main(int argc, char *argv[]) {
    int V = 3;
    Graph G = createGraph(V);
    addEdge(G, 1, 2, 4);
    addEdge(G, 0, 2, 1);
    addEdge(G, 0, 1, 2);
    showAdjList(G);
    showAdjMatrix(G);
    liberaGrafo(G);
    return 1;
}