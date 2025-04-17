#ifndef INPUT_H
#define INPUT_H

#include "array.h"

/**
 * @brief 从用户输入中读取多项式的系数和指数
 * @param n 多项式的项数
 * @return 包含输入数据的结构体指针
 */
typedef struct input{
    array *xi;          /**< 系数数组 */
    array *zhi;         /**< 指数数组 */
    int flag;
    int base;           // 使用数组存储时的基址,等于最小指数
    int max;
} input;


input *poly_input();
#endif