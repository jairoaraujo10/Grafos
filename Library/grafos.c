#include "grafos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

CaminhosMinimos caminhos_init(Graph G, vertex src) {
    int i;
    CaminhosMinimos C = malloc(sizeof(CaminhosMinimos));
    C->array = calloc(G->V, sizeof(caminhos));
    C->V = G->V;
    C->src = src;
    for (i = 0; i < G->V; i++) {
        C->array[i].pai = -1;
        C->array[i].weight = infn;
    }
    C->array[src].pai = src;
    C->array[src].weight = 0;
    return C;
}

void imprimeCaminhos(CaminhosMinimos C) {
    int i, j;
    for (i = 0; i < C->V; i++) {
        j = i;
        if (C->array[j].pai != -1) {
            do {
                if (j != i) printf(" <- ");
                printf("%d", j);
                j = C->array[j].pai;
            } while (j != C->src);
            printf(" <- %d", C->src);
            printf("\n");
        }
    }
    printf("\n");
}

void liberaCaminhos(CaminhosMinimos C) {
    free(C->array);
    free(C);
}

minHeapNode newMinHeapNode(vertex v, CaminhosMinimos C) {
    minHeapNode minHN = malloc(sizeof(minHeapNode));
    minHN->v = v;
    minHN->dist = C->array[v].weight;
    return minHN;
}

minHeap createMinHeap(int capacity) {
    minHeap minH = (minHeap) malloc(sizeof(minHeap));
    minH->pos = malloc((capacity) * sizeof(int));
    minH->size = 0;
    minH->array = malloc((capacity) * sizeof(caminhos));
    return minH;
}

void swapMinHeapNode(minHeapNode *a, minHeapNode *b) {
    minHeapNode t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(minHeap minH, vertex idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minH->size &&
        minH->array[left]->dist < minH->array[smallest]->dist)
        smallest = left;

    if (right < minH->size &&
        minH->array[right]->dist < minH->array[smallest]->dist)
        smallest = right;

    if (smallest != idx) {
        minHeapNode smallestNode = minH->array[smallest];
        minHeapNode idxNode = minH->array[idx];

        minH->pos[smallestNode->v] = idx;
        minH->pos[idxNode->v] = smallest;

        swapMinHeapNode(&minH->array[smallest], &minH->array[idx]);

        minHeapify(minH, smallest);
    }
}

int isEmpty(minHeap minH) {
    return minH->size == 0;
}

minHeapNode extractMin(minHeap minH) {
    if (isEmpty(minH))
        return NULL;

    minHeapNode root = minH->array[0];

    minHeapNode lastNode = minH->array[minH->size - 1];
    minH->array[0] = lastNode;

    minH->pos[root->v] = minH->size - 1;
    minH->pos[lastNode->v] = 0;

    --minH->size;
    minHeapify(minH, 0);

    return root;
}

void decreaseKey(minHeap minH, vertex v, float weight) {
    vertex i = minH->pos[v];

    minH->array[i]->dist = weight;

    while (i && minH->array[i]->dist < minH->array[(i - 1) / 2]->dist) {
        minH->pos[minH->array[i]->v] = (i - 1) / 2;
        minH->pos[minH->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minH->array[i], &minH->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

int isInMinHeap(minHeap minH, vertex v) {
    if (minH->pos[v] < minH->size)
        return 1;
    return 0;
}

int Relax(vertex u, vertex v, float w, CaminhosMinimos C) {
    float dv, du;
    du = C->array[u].weight;
    dv = C->array[v].weight;

    // Se d(v) > d(u) + w(u,v)
    if (du != infn && dv > du + w) {
        C->array[v].weight = du + w;
        C->array[v].pai = u;
        return 1;
    }
    return 0;
}

CaminhosMinimos Dijkstra(Graph G, vertex src) {
    vertex u, v;

    // Cria uma lista de caminhos e atribui os valores iniciais dos pesos das arestas e dos predecessores dos vértices;
    CaminhosMinimos C = caminhos_init(G, src);

    // O algoritmo é implementado usando uma heap binária
    minHeap minH = createMinHeap(G->V);

    // Inicializa os valores da heap para todos os vertices. Cada nó contém o número
    // do vértice e o valor da distância. Torne o valor dist do vértice src como 0
    // para que ele seja extraído primeiro.
    for (v = 0; v < G->V; ++v) {
        minH->array[v] = newMinHeapNode(v, C);
        minH->pos[v] = v;
    }

    decreaseKey(minH, src, C->array[src].weight);

    // Inicialmente o tamanho da pilha mínima é igual a V(G)
    minH->size = G->V;

    // O heap agora possui todos os vértices cuja distancia mínima não for determinada
    while (!isEmpty(minH)) {
        // Extrai o vértice com o valor mínimo da distância e o armazena em u;
        minHeapNode minHN = extractMin(minH);
        u = minHN->v;

        // Percorre todos os vértices adjacentes de u e atualize seus valores de distância realizando um relaxamento
        link pCrawl = G->adjList[u].head;
        while (pCrawl != NULL) {
            v = pCrawl->v;
            if (isInMinHeap(minH, v) && Relax(u, v, pCrawl->weight, C)) {
                // Se d(pai) > d(u) + w(u, pai), realiza um decreaseKey na tabela;
                decreaseKey(minH, v, C->array[v].weight);
            }
            pCrawl = pCrawl->next;
        }
    }
    return C;
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