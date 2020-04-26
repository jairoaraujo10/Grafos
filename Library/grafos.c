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

CaminhosMinimos caminhos_init(Graph G, vertex s) {
    CaminhosMinimos C = calloc(G->V, sizeof(CaminhosMinimos));
    int i;
    for (i = 0; i < G->V; i++) {
        C[i].pai = -1;
        C[i].weight = infn;
    }
    C[s].pai = s;
    C[s].weight = 0;
    return C;
}

void imprimeCaminhos(Graph G, vertex src, CaminhosMinimos C) {
    int V = G->V;
    int i, j;
    for (i = 0; i < V; i++) {
        j = i;
        if (C[j].pai != -1) {
            do {
                if (j != i) printf(" <- ");
                printf("%d", j);
                j = C[j].pai;
            } while (j != src);
            printf(" <- %d", src);
            printf("\n");
        }
    }
    printf("\n");
}

void liberaCaminhos(CaminhosMinimos C) {
    free(C);
}

int Relax(vertex u, vertex v, float w, CaminhosMinimos C) {
    float dv, du;
    du = C[u].weight;
    dv = C[v].weight;

    // Se d(v) > d(u) + w(u,v)
    if (du != infn && dv > du + w) {
        C[v].weight = du + w;
        C[v].pai = u;
        return 1;
    }
    return 0;
}

CaminhosMinimos BellmanFord(Graph G, vertex src) {
    int i;
    vertex u, v;

    // Inicializa um array de caminhos;
    CaminhosMinimos C = caminhos_init(G, src);

    // Para i de 1 até V(G) - 1 faça:
    link p;
    for (i = 0; i <= G->V - 1; i++) {
        // Para toda aresta (u,v) pertencente a E(G), execute o relax;
        for (u = 0; u < G->V; u++) {
            p = G->adjList[u].head;
            while (p != NULL) {
                v = p->v;
                Relax(u, v, p->weight, C);
                p = p->next;
            }
        }
    }

    // Verifica se o grafo possui ciclo de peso negativo;
    // Para cada (u,v) pertencente a E(G), realiza o relax;
    for (u = 0; u < G->V; u++) {
        p = G->adjList[u].head;
        while (p != NULL) {
            v = p->v;
            // Se o relax retornar 1, o grafo possui cíclo de peso negativo;
            if (Relax(u, v, p->weight, C)) {
                printf("\n------- O grafo possui ciclo de peso negativo -------\n\n");
                exit(-1);
            }
            p = p->next;
        }
    }
    return C;
}