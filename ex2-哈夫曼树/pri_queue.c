#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "pri_queue.h"

Heap *create_heap(Heap *heap)
{
    heap = (Heap *)malloc(sizeof(Heap));
    heap->size = 1;
    heap->p = (Node *)malloc(heap->size * sizeof(Node));
    heap->count = 0;
    return heap;
}

void free_heap(Heap *heap)
{
    if (!heap) return;
    free(heap->p);
    free(heap);
    return;
}
void down_heapify(Heap *heap, int index)
{
    if (index >= heap->count)
        return;
    int left = index * 2 + 1;
    int right = index * 2 + 2;
    int leftflag = 0, rightflag = 0;

    int minimum = (heap->p + index)->num;
    if (left < heap->count && minimum > (heap->p + left)->num)
    {
        minimum = (heap->p + left)->num;
        leftflag = 1;
    }
    if (right < heap->count && minimum > (heap->p + right)->num)
    {
        minimum = (heap->p + right)->num;
        leftflag = 0;
        rightflag = 1;
    }
    if (leftflag)
    {
        Node temp = *(heap->p + index);
        *(heap->p + index) = *(heap->p + left);
        *(heap->p + left) = temp;
        down_heapify(heap, left);
    }
    if (rightflag)
    {
        Node temp = *(heap->p + index);
        *(heap->p + index) = *(heap->p + right);
        *(heap->p + right) = temp;
        down_heapify(heap, right);
    }
}

void up_heapify(Heap *heap, int index)
{
    int parent = (index - 1) / 2;
    if (parent < 0)
        return;
    if ((heap->p + index)->num < (heap->p + parent)->num)
    {
        Node temp = *(heap->p + index);
        *(heap->p + index) = *(heap->p + parent);
        *(heap->p + parent) = temp;
        up_heapify(heap, parent);
    }
}

void push(Heap *heap, Node* x)
{
    if (heap->count >= heap->size)
    {
        heap->size *= 2;
        heap->p = (Node *)realloc(heap->p, (heap->size) * sizeof(Node));
    }
    (heap->p + heap->count)->num = x->num;
    (heap->p + heap->count)->symbol = x->symbol;
    (heap->p + heap->count)->left = x->left;
    (heap->p + heap->count)->right = x->right;
    heap->count++;
    up_heapify(heap, heap->count - 1);
}

void pop(Heap *heap)
{
    if (heap->count == 0)
        return;
    heap->count--;
    Node temp = *(heap->p + heap->count);
    *(heap->p + heap->count) = *(heap->p);
    *(heap->p) = temp;
    down_heapify(heap, 0);
    if (4 * heap->count <= heap->size && heap->size > 1)
    {
        heap->size /= 2;
        heap->p = (Node *)realloc(heap->p, (heap->size) * sizeof(Node));
    }
}

Node top(Heap *heap)
{
    if (heap->count != 0)
        return *(heap->p);
    else
    {
        Node empty_node = {0, INT_MAX};
        return empty_node;
    }
}

int empty(Heap *heap)
{
    if (heap->count != 0)
        return 0;
    else
        return 1;
}

int size(Heap *heap) { return heap->count; }