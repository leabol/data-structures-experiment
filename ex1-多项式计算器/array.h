#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define valueType int

typedef enum{
    OK = 0,
    FAIL = 1,
    ARRAY_NULL = 2,
    ARRAY_OUT_OF_BOUNDS = 3,
    ENLARGE_FAIL = 4
}status;


typedef struct {
    valueType *data;
    size_t size;
    size_t capacity;
}array;

/**
 * @brief 创建一个新的动态数组
 * @param capacity 数组的初始容量
 * @return 返回新创建的数组指针，如果内存分配失败则返回NULL
 */
array *array_new(size_t capacity);

/**
 * @brief 扩大数组的容量
 * @param arr 需要扩容的数组指针
 * @return 扩容后的数组指针，如果内存分配失败则返回NULL
 * 
 * 该函数将数组容量扩大为原来的两倍
 */
array *array_enlarge(array *arr);

/**
 * @brief 在数组末尾添加元素
 * @param arr 数组指针
 * @param value 要添加的值
 * @return 操作状态码
 * 
 * 该函数实际上调用array_insert实现添加功能
 */
status array_append(array *arr, valueType value);

/**
 * @brief 在指定位置插入元素
 * @param arr 数组指针
 * @param index 要插入的位置索引
 * @param value 要插入的值
 * @return 操作状态码
 * 
 * 若数组已满，会自动扩容。如果index大于当前数组大小，则返回越界错误
 */
status array_insert(array *arr, size_t index, valueType value);

/**
 * @brief 打印数组所有元素
 * @param arr 数组指针
 * 
 * 若数组为空，则打印提示信息。否则打印数组中所有元素
 */
void array_print(array *arr);

/**
 * @brief 移除数组中指定位置的元素
 * @param arr 数组指针
 * @param index 要移除的元素索引
 * @return 操作状态码
 * 
 * 如果index大于或等于当前数组大小，则返回越界错误
 */
status array_remove(array *arr, size_t index);

/**
 * @brief 获取数组中指定位置的元素值
 * @param arr 数组指针
 * @param index 要获取的元素索引
 * @param value 用于存储获取的值的指针
 * @return 操作状态码
 * 
 * 如果index大于或等于当前数组大小，则返回越界错误
 */
status array_get(array *arr, size_t index, valueType *value);

/**
 * @brief 设置数组中指定位置的元素值
 * @param arr 数组指针
 * @param index 要设置的元素索引
 * @param value 要设置的新值
 * @return 操作状态码
 * 
 * 如果index大于或等于当前数组大小，则返回越界错误
 */
status array_set(array *arr, size_t index, valueType value);

/**
 * @brief 销毁数组并释放所有内存
 * @param arr 要销毁的数组指针
 * @return 操作状态码
 * 
 * 该函数会释放数组数据区域和数组结构体的内存
 */
status array_destroy(array *arr);

/**
 * @brief 清空数组中的所有元素
 * @param arr 数组指针
 * @return 操作状态码
 * 
 * 该函数不会释放内存，只是将数组大小设置为0
 */
status array_clear(array *arr);


#endif
