#include "minHeap.h"
#include <stdlib.h>
#include <string.h>

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