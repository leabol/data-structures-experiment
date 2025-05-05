/**
 * @file pri_queue.h
 * @brief 优先队列的实现，基于最小堆
 * 
 * 该文件定义了一个基于最小堆的优先队列数据结构及其操作函数，
 * 队列中的元素按照优先级（值小的优先级高）进行组织
 */

#ifndef PRI_QUEUE_H
#define PRI_QUEUE_H

#include <limits.h>
typedef struct char_node
{
    int symbol; 
    int num;
    struct char_node *left;
    struct char_node *right;
}Node;

/**
 * @brief 最小堆结构体定义
 */
typedef struct min_heap
{
    Node *p;     // 存储堆元素的数组
    int size;   // 当前分配的堆大小
    int count;  // 当前堆中的元素数量
} Heap;

/**
 * @brief 创建一个新的最小堆
 * @param heap 指向堆结构体的指针
 * @return 指向创建的堆结构体的指针
 */
Heap *create_heap(Heap *heap);

/**
 * @brief 向下调整堆
 * @param heap 指向堆结构体的指针
 * @param index 开始调整的索引
 */
void down_heapify(Heap *heap, int index);

/**
 * @brief 向上调整堆
 * @param heap 指向堆结构体的指针
 * @param index 开始调整的索引
 */
void up_heapify(Heap *heap, int index);

/**
 * @brief 向堆中插入一个元素
 * @param heap 指向堆结构体的指针
 * @param x 要插入的值
 */
void push(Heap *heap, Node* x);

/**
 * @brief 移除堆顶元素
 * @param heap 指向堆结构体的指针
 */
void pop(Heap *heap);

/**
 * @brief 获取堆顶元素
 * @param heap 指向堆结构体的指针
 * @return 堆顶元素的值，如果堆为空则返回 INT_MIN
 */
Node top(Heap *heap);

/**
 * @brief 检查堆是否为空
 * @param heap 指向堆结构体的指针
 * @return 如果堆为空返回1，否则返回0
 */
int empty(Heap *heap);

/**
 * @brief 获取堆中元素数量
 * @param heap 指向堆结构体的指针
 * @return 堆中的元素数量
 */
int size(Heap *heap);

#endif /* PRI_QUEUE_H */