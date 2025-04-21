#ifndef POLY_ARR_H
#define POLY_ARR_H

// 前向声明input结构体
struct input;
typedef struct input input;

/*
 *  arr[0]: 保存数组实际大小（有效项数）
 *  arr[1]: 保存基准值（用于映射负指数）
 *  arr[2+i]: 保存对应指数的系数
 */

/**
 * @brief 从输入数据创建多项式数组表示
 * @param in 包含系数和指数的输入数据结构
 * @return 创建的多项式数组，失败时返回NULL
 * 
 */
int *poly_arr_create(input *in);

/**
 * @brief 计算两个多项式的和
 * @param a 第一个多项式数组
 * @param b 第二个多项式数组
 * @return 结果多项式数组，失败时返回NULL
 * 
 * 将两个多项式相加，返回新的多项式数组。结果多项式的指数范围是输入多项式指数范围的并集。
 */
int *poly_arr_add(const int *a, const int *b);

/**
 * @brief 计算两个多项式的差
 * @param a 第一个多项式数组
 * @param b 第二个多项式数组
 * @return 结果多项式数组，失败时返回NULL
 * 
 * 从第一个多项式中减去第二个多项式，返回新的多项式数组。
 */
int *poly_arr_neg(const int *a, const int *b);

/**
 * @brief 计算两个多项式的积
 * @param a 第一个多项式数组
 * @param b 第二个多项式数组
 * @return 结果多项式数组，失败时返回NULL
 * 
 * 将两个多项式相乘，返回新的多项式数组。结果多项式的指数范围由输入多项式的指数范围决定。
 */
int *poly_arr_mul(const int *a, const int *b);

/**
 * @brief 打印多项式
 * @param poly 要打印的多项式数组
 * 
 * 打印多项式的各项，格式为: ax^n+bx^m+...
 * 如果系数为1，则只打印x^n。
 */
void poly_arr_print(const int *poly);

/**
 * @brief 释放多项式数组占用的内存
 * @param poly 要释放的多项式数组
 * 
 * 释放为多项式分配的所有内存资源。
 */
void poly_arr_free(int *poly);

#endif /* POLY_ARR_H */