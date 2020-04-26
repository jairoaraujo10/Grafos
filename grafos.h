#ifndef GRAFOS_GRAFOS_H
#define GRAFOS_GRAFOS_H

#define vertex int
#define infn 999999

typedef struct Node *link;

struct Node {
    vertex v;
    float weight;
    link next;
};

typedef struct AdjList *adj;

struct AdjList {
    link head;
};

struct graph {
    int V;
    int E;
    adj adjList;
    float **adjMat;
};

typedef struct graph *Graph;

// Retorna um grafo com V vertices
Graph createGraph(int V);

// Retorna o ponteiro para um novo nó que será adicionado a lista de adjacencia
link newAdjListNode(vertex v, float weight);

// Adiciona uma aresta no grafo G de u a v com peso weight
void addEdge(Graph G, vertex u, vertex v, float weight);

// Imprime a lista de adjacência do grafo G
void showAdjList(Graph G);

// Imprime a matriz de adjacência do grafo G
void showAdjMatrix(Graph G);

// Libera o espaço alocado para o grafo G
void liberaGrafo(Graph G);

#endif //GRAFOS_GRAFOS_H