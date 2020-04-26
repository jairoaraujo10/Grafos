#include "Library/grafos.h"

int main(int argc, char *argv[]) {

    int V = 8;
    Graph G = createGraph(V);
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

    vertex src = 3;
    CaminhosMinimos C = BellmanFord(G, src);
    imprimeCaminhos(G, src, C);

    liberaCaminhos(C);
    liberaGrafo(G);

    return 1;
}