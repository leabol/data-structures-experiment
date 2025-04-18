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
    int num;
} input;

// 前向声明多项式类型
struct poly;
typedef struct poly poly;

// 包含其他头文件
#include "poly_arr.h"
#include "poly_list.h"

void print_input(input* in);
input *poly_input();

/**
 * @brief 判断多项式运算应该使用哪种数据结构
 * @param input_a 第一个多项式的输入结构
 * @param input_b 第二个多项式的输入结构
 * @return 1表示使用数组，2表示使用链表
 */
int determine_storage_type(input *input_a, input *input_b);

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