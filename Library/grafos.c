#include "grafos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define vertex int
#define infn 999999

Graph createGraph(int V) {
    vertex u, v;
    Graph G = malloc(sizeof(Graph));
    G->V = V;
    G->E = 0;

    // Crie um array de listas de adjacência. O tamanho da array será V.
    G->adjList = malloc(V * sizeof(adj));

    // Inicializa cada lista de adjacência como vazia - head = NULL
    for (u = 0; u < V; u++)
        G->adjList[u].head = NULL;

    // Crie uma matriz de adjacência com valores infinitos(sem arestas). O tamanho da matriz será VxV.
    G->adjMat = (float **) calloc(G->V, sizeof(float *));
    for (u = 0; u < G->V; u++) {
        G->adjMat[u] = (float *) calloc(G->V, sizeof(float));
        for (v = 0; v < G->V; v++) {
            G->adjMat[u][v] = infn;
        }
    }

    return G;
}

link newAdjListNode(vertex v, float weight) {
    link newNode = malloc(sizeof(link));
    newNode->v = v;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

void addEdge(Graph G, vertex u, vertex v, float weight) {
    link newNode = newAdjListNode(v, weight);
    newNode->next = G->adjList[u].head;
    G->adjList[u].head = newNode;
    G->adjMat[u][v] = weight;
    G->E++;
}

void showAdjList(Graph G) {
    vertex a;
    link b;
    printf("Lista de adjacência:\n\t");
    for (a = 0; a < G->V; a++) {
        printf("%d:", a);
        for (b = G->adjList[a].head; b != NULL; b = b->next) {
            printf(" -> %d (Peso: %.2f)", b->v, b->weight);
        }
        printf("\n\t");
    }
    printf("\n");
}

void showAdjMatrix(Graph G) {
    vertex u, v;
    printf("Matriz de adjacência:\n\t");
    for (u = 0; u < G->V; u++) {
        for (v = 0; v < G->V; v++) {
            if (G->adjMat[u][v] == infn) printf("infn\t");
            else printf("%.2f\t", G->adjMat[u][v]);
        }
        printf("\n\t");
    }
    printf("\n");
}

void liberaGrafo(Graph G) {
    vertex v;
    link atual, proxNode;
    for (v = 0; v < G->V; v++) {
        atual = G->adjList[v].head;
        while (atual != NULL) {
            proxNode = atual->next;
            free(atual);
            atual = proxNode;
        }
        free(G->adjMat[v]);
    }
    free(G->adjList);
    free(G->adjMat);
    free(G);
}