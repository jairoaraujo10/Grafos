#include "grafos.h"
#include "minHeap.h"
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
    if (G == NULL) return;
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
    if (G == NULL) return;
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

int arestaNegativa(Graph G) {
    int u, v;
    for (u = 0; u < G->V; u++) {
        for (v = 0; v < G->V; v++) {
            if (G->adjMat[u][v] < 0) return 1;
        }
    }
    return 0;
}

void liberaGrafo(Graph G) {
    if (G == NULL) return;
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
    if (C == NULL) return;
    int i, j;
    for (i = 0; i < C->V; i++) {
        j = i;
        if (C->array[j].pai != -1) {
            printf("\t");
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
    if (C == NULL) return;
    free(C->array);
    free(C);
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

    if (arestaNegativa(G)) {
        printf("------- O grafo possui aresta de peso negativo -------\n\n");
        return NULL;
    }

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
                printf("------- O grafo possui ciclo de peso negativo -------\n\n");
                return NULL;
            }
            p = p->next;
        }
    }
    return C;
}

Graph GetSourceGraph(Graph G) {
    int i;
    Graph G0 = createGraph(G->V + 1);
    link p;
    for (i = 1; i < G0->V; i++)
        addEdge(G0, 0, i, 0);

    for (i = 0; i < G->V; i++) {
        p = G->adjList[i].head;
        while (p != NULL) {
            addEdge(G0, i + 1, p->v + 1, p->weight);
            p = p->next;
        }
    }
    return G0;
}

CaminhosMinimos *Johnson(Graph G) {
    int i, j;

    // Crie um array de caminhos mínimos
    CaminhosMinimos *CMin = calloc(G->V, sizeof(CaminhosMinimos));
    for (i = 0; i < G->V; i++)
        CMin[i] = caminhos_init(G, i);

    // Calcula G' a partir de G;
    Graph G0 = GetSourceGraph(G);

    // Execute o algoritmo de Bellman-Ford em G' a partir do vértice s para verificar
    // se o grafo possui ciclos. Se G' não possuir ciclo de peso negativo, calcule
    // os valores de h(v);
    CaminhosMinimos CaminhosG0 = BellmanFord(G0, 0);

    // Cria um grafo G1, onde:
    //  V(G1) = V(G)
    //  E(G1) = E(G)
    Graph G1 = createGraph(G->V);

    float W;
    link p;
    // Para cada (u,v) pertencente a E(G) faça:
    for (i = 1; i < G0->V; i++) {
        p = G0->adjList[i].head;
        while (p != NULL) {
            // W'(u,v) = W(u,v) + h(u) - h(v);
            W = p->weight + CaminhosG0->array[i].weight - CaminhosG0->array[p->v].weight;
            if (W > infn)
                W = infn;
            // W'(u,v) é o peso atualizado das arestas de G1 que ligam os vértices u a v;
            addEdge(G1, i - 1, p->v - 1, W);
            p = p->next;
        }
    }
    free(p);

    // Para cada vértice v pertencente a G1 faça:
    for (i = 0; i < G1->V; i++) {
        // Executa o algoritmo de Dijkstra no grafo G1 a partir de u(i);
        CMin[i] = Dijkstra(G1, i);
        // Desfaz a operação que atualizou o peso das arestas de G1;
        // h(u,v) = h'(u,v) + h(v+1) - h(u+1)
        for (j = 0; j < G1->V; j++) {
            if (CMin[i]->array[j].weight == infn)
                CMin[i]->array[j].weight = infn;
            else
                CMin[i]->array[j].weight = CMin[i]->array[j].weight + CaminhosG0->array[j + 1].weight - CaminhosG0->array[i + 1].weight;
        }
    }

    liberaGrafo(G0);
    liberaGrafo(G1);
    liberaCaminhos(CaminhosG0);

    // Retorne um array de caminhos mínimos;
    return CMin;
}