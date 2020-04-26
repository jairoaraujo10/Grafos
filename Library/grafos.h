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

typedef struct {
    link head;
} *adj;

typedef struct {
    int V;
    int E;
    adj adjList;
    float **adjMat;
} *Graph;

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

typedef struct {
    vertex pai;
    float weight;
} *caminhos;

typedef struct {
    caminhos array;
    int V;
    int src;
} *CaminhosMinimos;

CaminhosMinimos caminhos_init(Graph G, vertex s);

void imprimeCaminhos(CaminhosMinimos C);

void liberaCaminhos(CaminhosMinimos C);

// Função de relaxamento entre os vértices u e v.
int Relax(vertex u, vertex v, float w, CaminhosMinimos C);

// Algoritmo de Dijkstra: Recebe um grafo G, um vértice src partir do qual
// serão calculados os menores caminhos até os demais vértices. Retorna um array
// de caminhos mínimos (com peso do caminho e predecessor do vértice do índice).
// Se o grafo  possuir ciclo de peso negativo, é impresso um alerta e a execuçãodo
// código é interrompida.
CaminhosMinimos Dijkstra(Graph G, vertex src);

// Algoritmo de Bellman Ford: Recebe um grafo G, um vértice src partir do qual
// serão calculados os menores caminhos até os demais vértices. Retorna um array
// de caminhos mínimos (com peso do caminho e predecessor do vértice do índice).
// Se o grafo  possuir ciclo de peso negativo, é impresso um alerta e a execuçãodo
// código é interrompida.
CaminhosMinimos BellmanFord(Graph G, vertex src);

#endif //GRAFOS_GRAFOS_H