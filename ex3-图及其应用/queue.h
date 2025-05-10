#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

/**
 * @brief 扩容倍数
 * 当队列空间不足时，新容量为原容量的 BIGGER 倍
 */
#define BIGGER 2

/**
 * @brief 队列中存储的值类型
 */
typedef int valueType;

/**
 * @brief 函数指针类型，用于打印队列元素
 */
typedef void (*print_func)(valueType);

/**
 * @brief 队列结构体定义
 * 
 * 实现了一个基于数组的循环队列
 */
typedef struct queue {
    valueType *data;    /**< 存储队列元素的数组 */
    size_t front;       /**< 队首索引 */
    size_t tail;        /**< 队尾索引 */
    size_t size;        /**< 当前队列中的元素数量 */
    size_t capacity;    /**< 队列最大容量 */
} queue;

/**
 * @brief 创建新队列
 * 
 * @param capacity 队列初始容量
 * @return queue* 成功返回队列指针，失败返回NULL
 */
queue* queue_new(size_t capacity);

/**
 * @brief 检查队列是否为空
 * 
 * @param queue 要检查的队列
 * @return int 为空返回1，否则返回0
 */
int queue_isEmpty(queue *queue);

/**
 * @brief 检查队列是否已满
 * 
 * @param queue 要检查的队列
 * @return int 已满返回1，否则返回0
 */
int queue_isFull(queue *queue);

/**
 * @brief 获取队列容量
 * 
 * @param queue 目标队列
 * @return size_t 队列容量
 */
size_t queue_capacity(queue *queue);

/**
 * @brief 队列扩容
 * 当队列满时，扩大队列容量并重新排列元素
 * 
 * @param queue 要扩容的队列
 * @return int 成功返回0，失败返回1
 */
int queue_resize(queue *queue);

/**
 * @brief 元素入队
 * 
 * @param queue 目标队列
 * @param value 要入队的值
 * @return int 成功返回0，失败返回1
 */
int queue_enqueue(queue *queue, valueType value);

/**
 * @brief 元素出队
 * 
 * @param queue 目标队列
 * @param value 用于存储出队元素的指针
 * @return int 成功返回0，失败返回1
 */
int queue_dequeue(queue *queue, valueType *value);

/**
 * @brief 打印队列中所有元素
 * 
 * @param queue 要打印的队列
 */
void queue_print(queue *queue);

/**
 * @brief 使用自定义打印函数打印队列中所有元素
 * 
 * @param queue 要打印的队列
 * @param printer 自定义打印函数
 */
void queue_print_with_func(queue *queue, print_func printer);

/**
 * @brief 查看队首元素但不出队
 * 
 * @param queue 目标队列
 * @param value 用于存储队首元素的指针
 * @return int 成功返回0，失败返回1
 */
int queue_peek(queue *queue, valueType *value);

/**
 * @brief 获取队列当前元素数量
 * 
 * @param queue 目标队列
 * @return size_t 元素数量
 */
size_t queue_getsize(queue *queue);

/**
 * @brief 清空队列中的所有元素
 * 
 * @param queue 要清空的队列
 */
void queue_clear(queue *queue);

/**
 * @brief 销毁队列并释放内存
 * 
 * @param queue_ptr 指向队列指针的指针
 * @return int 成功返回0，失败返回1
 */
int queue_destroy(queue** queue_ptr);

/**
 * @brief 对队列中每个元素执行操作
 * 
 * @param queue 目标队列
 * @param func 要执行的函数
 */
void queue_foreach(queue *queue, void (*func)(valueType));

#endif /* QUEUE_H */