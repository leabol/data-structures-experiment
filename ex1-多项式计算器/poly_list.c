#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "poly_list.h"
#include "input.h"

/**
 * @brief 创建一个新的链表节点
 * @param data 指向项数据的指针
 * @return 新创建的节点指针，如果内存分配失败则返回NULL
 */
Node *createNode(term* data)
{
    Node *new_node = (Node*)malloc(sizeof(Node));
    if(new_node == NULL) return NULL;

    new_node -> data = data;

    new_node -> next = NULL;
    return new_node;
}

/**
 * @brief 将节点插入有序链表中
 * @param head 指向链表头指针的指针
 * @param node 要插入的节点
 * @param compare 比较函数，用于确定节点在链表中的位置
 * @param merge 合并函数，当找到相同项时用于合并项
 * @return 成功返回1，失败返回-1
 */
int insert(Node **head, Node *node, int (*compare)(Node *na, Node *nb),void (*merge)(Node *source, Node *dirct))
{
    if(head == NULL || node == NULL){
        return -1;
    }

    Node **cur = head;
    while (*cur != NULL && compare(node, *cur) < 0){//node < cur
        cur = &((*cur) ->next);
    }
    if (*cur != NULL && compare(node, *cur) == 0){
        merge(node, *cur);
        // 释放合并后不再需要的源节点及其数据
        term_free(node->data);
        free(node);
        return 1;
    }
    node -> next = *cur;
    *cur = node;

    return 1;
}

/**
 * @brief 销毁链表并释放所有节点和数据的内存
 * @param head 指向链表头指针的指针
 * @param distroy_data 用于释放项数据内存的函数
 * @return 总是返回0
 */
int destroyList(Node **head, void (*distroy_data)(term* data)){
    if (!head || !*head)
        return 0;
    Node *temp = *head;
    Node *next;
    while (temp){
        next = temp->next;
        distroy_data(temp->data);
        free(temp);
        temp = next;
    }
    *head = NULL;
    return 0;
}
static int compare(Node *na, Node *nb)
{
    return na->data->zhi - nb->data->zhi;
}

static void merge(Node *source, Node *direct)
{
    direct->data->xi += source->data->xi;
}

/**
 * @brief 初始化一个新的多项式
 * @return 指向新创建的多项式的指针，如果内存分配失败则返回NULL
 */
poly *poly_init()
{
    poly *new_poly = (poly*)malloc(sizeof(poly));
    if (!new_poly) return NULL;

    new_poly->terms = NULL;
    return new_poly;
}

term *term_create(int xi, int zhi)
{
    term *new_term = (term*)malloc(sizeof(term));
    if (!new_term) return NULL;

    new_term->xi  = xi;
    new_term->zhi = zhi;
    return new_term;
}

void term_free(term* term)
{
    if (!term) return;
    free(term);
}

void poly_free(poly* mypoly)
{
    if (!mypoly || !mypoly->terms) return;
    destroyList(&(mypoly->terms),term_free);
    free(mypoly);
}

/**
 * @brief 使用排序插入将节点添加到链表中
 * @param head 指向链表头指针的指针
 * @param node 要插入的节点
 * @return 成功返回1，失败返回-1
 */
static int insert_sort(Node **head, Node *node)
{
    if(head == NULL || node == NULL) return -1;

    Node **cur = head;
    while (*cur != NULL && compare(node, *cur) < 0){//node < cur
        cur = &((*cur) ->next);
    }

    if (*cur != NULL && compare(node, *cur) == 0){
        merge(node, *cur);
        // 释放合并后不再需要的源节点及其数据
        term_free(node->data);
        free(node);
        return 1;
    }

    node -> next = *cur;
    *cur = node;

    return 1;
}

void poly_add_term(poly* poly, int xi, int zhi)
{
    if (!poly || xi == 0) return;

    term* new_term = term_create(xi, zhi);
    if (!new_term) {
        printf("内存分配失败\n");
        return;
    }
    
    Node *new_node = createNode(new_term);
    if (!new_node) {
        printf("内存分配失败\n");
        term_free(new_term);
        return;
    }
    
    int erro = insert_sort(&poly->terms, new_node);
    if (erro == -1){
        printf("节点指针无效\n");
        // 不再使用exit(-1)，而是释放资源后返回
        term_free(new_term);
        free(new_node);
        return;
    }
}

/**
 * @brief 根据输入数据创建一个多项式链表
 * @param in 包含系数和指数的输入结构体
 * @return 新创建的多项式指针
 * 
 * 该函数会释放输入结构体的内存
 */
poly *poly_list_create(input* in)
{
    array *array_xi = in->xi;
    array *array_zhi = in->zhi;

    int xi, zhi, num;
    num = in->xi->size;
    poly *new_poly = poly_init();
    for (int i = 0; i < num; i++){
        array_get(array_xi, i, &xi);
        array_get(array_zhi, i, &zhi);
        poly_add_term(new_poly, xi, zhi);
    }
    // array_destroy(in->xi);
    // array_destroy(in->zhi);
    // free(in);

    return new_poly;
}

/**
 * @brief 打印多项式
 * @param poly 要打印的多项式指针
 * 
 * 按照标准数学表示法打印多项式。处理特殊情况如：
 * - 空多项式或所有系数为0时打印"0"
 * - 系数为1的非常数项不显示系数
 * - 一次项显示为"x"而不是"x^1"
 * - 正确处理正负号和加减符号
 */
void Poly_list_print(poly *poly)
{
    if (!poly || !poly->terms) {
        printf("0");
        return;
    }

    Node *cur = poly->terms;
    int first_term = 1;  // 标记是否是第一项
    
    while (cur) {
        int xi = cur->data->xi;
        int zhi = cur->data->zhi;
        
        // 跳过系数为0的项
        if (xi == 0) {
            cur = cur->next;
            continue;
        }
        
        // 处理符号
        if (first_term) {
            if (xi < 0) printf("-");
            first_term = 0;
        } else {
            printf(xi > 0 ? " + " : " - ");
        }
        
        // 打印系数（取绝对值）
        int abs_xi = xi > 0 ? xi : -xi;
        
        if (zhi == 0) {
            // 常数项
            printf("%d", abs_xi);
        } else {
            // 非常数项
            if (abs_xi != 1) {
                printf("%d", abs_xi);
            }
            
            if (zhi == 1) {
                printf("x");
            } else {
                printf("x^%d", zhi);
            }
        }
        
        cur = cur->next;
    }
    
    // 如果多项式为空（所有系数为0）
    if (first_term) {
        printf("0");
    }
}

/**
 * @brief 执行两个多项式的减法
 * @param poly1 第一个多项式
 * @param poly2 第二个多项式
 * @return 相减结果的新多项式
 */
poly *poly_neg(poly *poly1, poly *poly2)
{
    poly *resault = poly_init();
    Node *pa = poly1->terms;
    Node *pb = poly2->terms;
    const Node *temp = NULL;
    while (pa && pb){
        temp = NULL;
        int flag = 1;
        if (pa->data->zhi > pb->data->zhi){
            temp = pa;
            pa = pa->next;
        }else if (pa->data->zhi < pb->data->zhi){
            temp = pb;
            pb = pb->next;
            flag = -1;
        }else{
            int add_xi = pa->data->xi - pb->data->xi; 
            int add_zhi = pa->data->zhi; 
            poly_add_term(resault, add_xi,add_zhi);
            pa = pa->next;
            pb = pb->next;
            continue;
        }
        poly_add_term(resault, flag * temp->data->xi,temp->data->zhi);
    }
    while (pa){
        temp = pa;
        pa = pa->next;
        poly_add_term(resault, temp->data->xi,temp->data->zhi);
    }
    while (pb){
        temp = pb;
        pb = pb->next;
        poly_add_term(resault, -temp->data->xi,temp->data->zhi);
    }
    return resault;
}

/**
 * @brief 执行两个多项式的加法
 * @param poly1 第一个多项式
 * @param poly2 第二个多项式
 * @return 相加结果的新多项式
 */
poly *poly_add(poly *poly1, poly *poly2)
{
    poly *resault = poly_init();
    Node *pa = poly1->terms;
    Node *pb = poly2->terms;
    const Node *temp = NULL;
    while (pa && pb){
        temp = NULL;
        if (pa->data->zhi > pb->data->zhi){
            temp = pa;
            pa = pa->next;
        }else if (pa->data->zhi < pb->data->zhi){
            temp = pb;
            pb = pb->next;
        }else{
            int add_xi = pa->data->xi + pb->data->xi; 
            int add_zhi = pa->data->zhi; 
            poly_add_term(resault, add_xi,add_zhi);
            pa = pa->next;
            pb = pb->next;
            continue;
        }
        poly_add_term(resault, temp->data->xi,temp->data->zhi);
    }
    while (pa){
        temp = pa;
        pa = pa->next;
        poly_add_term(resault, temp->data->xi,temp->data->zhi);
    }
    while (pb){
        temp = pb;
        pb = pb->next;
        poly_add_term(resault, temp->data->xi,temp->data->zhi);
    }
    return resault;
}

/**
 * @brief 执行两个多项式的乘法
 * @param poly1 第一个多项式
 * @param poly2 第二个多项式
 * @return 相乘结果的新多项式
 */
poly *poly_mul(poly *poly1, poly *poly2)
{
    poly *result = poly_init();
    Node *pa = poly1->terms;
    while (pa){
       Node *pb = poly2->terms;
       while (pb){
        int new_xi = pa->data->xi * pb->data->xi;
        int new_zhi = pa->data->zhi + pb->data->zhi;
        poly_add_term(result, new_xi,new_zhi);
        
        pb = pb->next;
       }
       pa = pa->next;
    }
    return result;
}
