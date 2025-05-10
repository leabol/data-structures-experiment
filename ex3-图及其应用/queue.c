#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

queue* queue_new(size_t capacity)
{
    if (capacity == 0){
        printf("capacity must more then 0\n"); 
        return NULL;
    }
    queue *new_queue = (queue*)malloc(sizeof(queue));
    if (new_queue == NULL){
        printf("new_queue malloc is failed\n");
        return NULL;
    }
    valueType *data = (valueType*)malloc(sizeof(valueType) * capacity);
    if (data == NULL) {
        free(new_queue);
        printf("data array malloc is failed\n");
        return NULL;
    }
    new_queue->data = data;
    new_queue->front = 0;
    new_queue->tail = 0;
    new_queue->size = 0;
    new_queue->capacity = capacity;
    return new_queue;
}

int queue_isEmpty(queue *queue)
{
    if (!queue) 
        return 1;
    if (queue->size == 0)
        return 1;
    return 0;
}

int queue_isFull(queue *queue)
{
    if (queue == NULL) return 0;
    return queue->size == queue->capacity;
}

size_t queue_capacity(queue *queue)
{
    if (queue == NULL) return 0;
    return queue->capacity;
}

int queue_resize(queue *queue)
{
    if (queue == NULL){
        printf("queue is not exist\n");
        return 1;
    }
    if (queue->size == queue->capacity){
        // 计算新容量
        size_t newcapacity = queue->capacity * BIGGER;
        // 分配新内存
        valueType* newdata = (valueType*)malloc(sizeof(valueType) * newcapacity);
        if (newdata == NULL){
            printf("realloc is failed\n");
            return 1;
        }
        
        // 将原队列中的元素复制到新数组，同时重新排列元素顺序
        size_t qusize = queue->size;
        for (size_t i = 0; i < qusize; i++){
            // 计算原数组中元素的实际位置
            size_t j = (i + queue->front) % queue->capacity;
            // 按顺序复制到新数组
            newdata[i] = queue->data[j];
        }
        
        free(queue->data);
        queue->data = newdata;
        queue->front = 0;
        queue->tail = qusize;
        queue->capacity = newcapacity;
    }
    return 0;
}

int queue_enqueue(queue *queue, valueType value)
{
    if (queue == NULL){
        printf("queue is not exist\n");
        return 1;
    }
    // 如果队列已满，尝试扩容
    if (queue_resize(queue) == 1){
        printf("resize is failed\n");
        return 1;
    }
   
    queue->data[queue->tail] = value;
    queue->tail = (1 + queue->tail) % queue->capacity;
    queue->size++;
    return 0;
}

int queue_dequeue(queue *queue, valueType *value)
{
    if (queue == NULL){
        printf("queue is not exist\n");
        return 1;
    }
    if (queue->size == 0){
        printf("queue is empty\n");
        return 1;
    }
    
    *value = queue->data[queue->front];
    queue->front = (1 + queue->front) % queue->capacity;
    queue->size--;
    return 0;
}

void queue_print_with_func(queue *queue, print_func printer)
{
    if (queue == NULL) return;
    if (queue->size == 0){
        printf("###queue is empty\n");
        return;
    }
    
    printf("Queue: ");
    for (size_t i = 0; i < queue->size; i++){
        // 计算元素在数组中的实际位置
        size_t j = (queue->front + i) % queue->capacity;
        printer(queue->data[j]);
    }
    printf("\n");
}

static void print_int(valueType val)
{
    printf("%d ", val);
}

void queue_print(queue *queue)
{
    queue_print_with_func(queue, print_int);
}

int queue_peek(queue *queue, valueType *value)
{
    if (queue == NULL || queue->size == 0){
        printf("queue is empty or null\n");
        return 1;
    }
    *value = queue->data[queue->front];
    return 0;
}

size_t queue_getsize(queue *queue)
{
    if (queue == NULL) return 0;
    return queue->size;
}

void queue_clear(queue *queue)
{
    if (queue == NULL) return;
    queue->tail = queue->front;
    queue->size = 0;
}

int queue_destroy(queue** queue_ptr) 
{
    if (queue_ptr == NULL || *queue_ptr == NULL) return 1;
    
    queue* q = *queue_ptr;
    if (q->data != NULL) {
        free(q->data);
    }
    free(q);
    *queue_ptr = NULL;
    
    return 0;
}

void queue_foreach(queue *queue, void (*func)(valueType))
{
    if (queue == NULL || func == NULL) return;
    
    for (size_t i = 0; i < queue->size; i++) {
        size_t j = (queue->front + i) % queue->capacity;
        func(queue->data[j]);
    }
}