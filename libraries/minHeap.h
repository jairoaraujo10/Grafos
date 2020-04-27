#ifndef GRAFOS_MINHEAP_H
#define GRAFOS_MINHEAP_H

#include "grafos.h"

#define vertex int
#define infn 999999

// Estrutura que representa um nó da min heap
typedef struct {
    int v;
    float dist;
} minHeapNode;

// Estrutura que representa a min heap
typedef struct {
    int size;                       // Número de nós
    int *pos;
    minHeapNode **array;            // Ponteiro que aponta para a lista de caminhos
} minHeap;

minHeapNode *newMinHeapNode(vertex v, CaminhosMinimos *C);

minHeap *createMinHeap(int capacity);

void swapMinHeapNode(minHeapNode *a, minHeapNode *b);

void minHeapify(minHeap *minH, vertex idx);

int isEmpty(minHeap *minH);

minHeapNode *extractMin(minHeap *minH);

void decreaseKey(minHeap *minH, vertex v, float weight);

int isInMinHeap(minHeap *minH, vertex v);

#endif //GRAFOS_MINHEAP_H