#ifndef POLY_H
#define POLY_H

/**
 * @brief 多项式项结构，表示一个多项式的单个项
 */
typedef struct term {
    int xi;    /**< 系数 */
    int zhi;   /**< 指数 */
} term;
typedef struct Node {
    term *data;
    struct Node *next;
} Node;
// 包含其他头文件，必须在定义完 term 结构体后
#include "array.h"
#include "input.h"

/**
 * @brief 多项式结构，包含项的链表
 */
typedef struct poly {
    Node *terms;   /**< 多项式项的链表 */
} poly;

/**
 * @brief 创建一个新的多项式
 * @return 指向新创建的多项式的指针
 */
poly *poly_init();

/**
 * @brief 创建一个新的多项式项
 * @param xi 系数
 * @param zhi 指数
 * @return 指向新创建的多项式项的指针
 */
term *term_create(int xi, int zhi);

/**
 * @brief 释放一个多项式项的内存
 * @param term 要释放的多项式项指针
 */
void term_free(term* term);

/**
 * @brief 释放一个多项式的内存，包括其所有项
 * @param mypoly 要释放的多项式指针
 */
void poly_free(poly* mypoly);


/**
 * @brief 向多项式添加一个新项
 * @param poly 多项式指针
 * @param xi 系数
 * @param zhi 指数
 */
void poly_add_term(poly* poly, int xi, int zhi);

/**
 * @brief 打印多项式
 * @param poly 要打印的多项式指针
 */
void Poly_list_print(poly *poly);

/**
 * @brief 执行两个多项式的加法
 * @param poly1 第一个多项式
 * @param poly2 第二个多项式
 * @return 相加结果多项式的指针
 */
poly *poly_add(poly *poly1, poly *poly2);

/**
 * @brief 执行两个多项式的减法
 * @param poly1 第一个多项式
 * @param poly2 第二个多项式
 * @return 相减结果多项式的指针
 */
poly *poly_neg(poly *poly1, poly *poly2);

/**
 * @brief 执行两个多项式的乘法
 * @param poly1 第一个多项式
 * @param poly2 第二个多项式
 * @return 相乘结果多项式的指针
 */
poly *poly_mul(poly *poly1, poly *poly2);


poly *poly_list_create(input* in);



#endif /* POLY_H */