#ifndef GRAFOS_GRAFOS_H
#define GRAFOS_GRAFOS_H

#define vertex int
#define infn 999999

typedef struct Node link;

struct Node {
    vertex v;
    float weight;
    link *next;
};

typedef struct {
    link *head;
} adj;

typedef struct {
    int V;
    int E;
    adj *adjList;
    float **adjMat;
} Graph;

// Retorna um grafo com V vertices
Graph *createGraph(int V);

// Retorna o ponteiro para um novo nó que será adicionado a lista de adjacencia
link *newAdjListNode(vertex v, float weight);

// Adiciona uma aresta no grafo G de u a v com peso weight
void addEdge(Graph *G, vertex u, vertex v, float weight);

// Retorna um grafo com V vertices e arestas aleatórias
Graph *randonGraph(int V, int rate);

// Imprime a lista de adjacência do grafo G
void showAdjList(Graph *G);

// Imprime a matriz de adjacência do grafo G
void showAdjMatrix(Graph *G);

// Verifica se o grafo possui aresta de  peso  negativo
int arestaNegativa(Graph *G);

// Libera o espaço alocado para o grafo G
void liberaGrafo(Graph *G);

typedef struct {
    vertex pai;
    float weight;
} caminhos;

typedef struct {
    caminhos *array;
    int V;
    int src;
} CaminhosMinimos;

CaminhosMinimos *caminhos_init(Graph *G, vertex s);

void imprimeCaminhos(CaminhosMinimos *C);

void liberaCaminhos(CaminhosMinimos *C);

// Função de relaxamento entre os vértices u e v.
int Relax(vertex u, vertex v, float w, CaminhosMinimos *C);

// Algoritmo de Dijkstra: Recebe um grafo G, um vértice src partir do qual serão
// calculados os menores caminhos até os demais vértices. etorna os caminhos mínimos.
// Se o grafo  possuir aresta de peso negativo, é impresso um alerta e returnado NULL.
CaminhosMinimos *Dijkstra(Graph *G, vertex src);

// Algoritmo de Bellman Ford: Recebe um grafo G, um vértice src partir do qual serão
// calculados os menores caminhos até os demais vértices. Retorna os caminhos mínimos.
// Se o grafo  possuir ciclo de peso negativo, é impresso um alerta e returnado NULL.
CaminhosMinimos *BellmanFord(Graph *G, vertex src);

// Retorna um grafo que contém todos os vértices de G adicionado de um
// vértice u conectado aos demais por arestas de peso 0, ou seja,
// w(u,v) = 0 para todos vértices v pertencentes a G;
Graph *GetSourceGraph(Graph *G);

// Algoritmo de Johnson: Recebe um grafo G. Retorna um array que contém os caminhos
// mínimos entre todos os pares de vértice. Se o grafo  possuir ciclo de peso
// negativo, é impresso um alerta e returnado NULL.
CaminhosMinimos **Johnson(Graph *G);

#endif //GRAFOS_GRAFOS_H