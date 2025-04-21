#ifndef INPUT_H
#define INPUT_H

#include "array.h"

typedef struct input{
    array *xi;          /**< 系数数组 */
    array *zhi;         /**< 指数数组 */
    int base;           // 使用数组存储时的基址,等于最小指数
    int max;
    int num;
} input;

void print_input(input* in);

void print_info(input *input_a, input *input_b);

/**
 * @brief 处理输入并获取多项式结构
 * @return 返回输入的多项式结构
 */
input *get_poly_input();

/**
 * @brief 计算两个多项式的和
 * @param input_a 第一个多项式输入
 * @param input_b 第二个多项式输入
 */
void calculate_add(input *input_a, input *input_b);

/**
 * @brief 计算两个多项式的差
 * @param input_a 第一个多项式输入
 * @param input_b 第二个多项式输入
 */
void calculate_neg(input *input_a, input *input_b);

/**
 * @brief 计算两个多项式的积
 * @param input_a 第一个多项式输入
 * @param input_b 第二个多项式输入
 */
void calculate_mul(input *input_a, input *input_b);


#endif