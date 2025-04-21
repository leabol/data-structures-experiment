#include "input.h"
#include "poly_arr.h"
#include "poly_list.h"
#include <limits.h> 

/**
 * @brief 判断多项式运算应该使用哪种数据结构
 * @param input_a 第一个多项式的输入结构
 * @param input_b 第二个多项式的输入结构
 * @return 1表示使用数组，2表示使用链表
 */
static int determine_storage_type(input *input_a, input *input_b) {
    if (!input_a || !input_b) {
        return 2; // 空指针情况下默认使用链表
    }
    
    // 计算多项式的指数范围
    int max = input_a->max > input_b->max ? input_a->max : input_b->max;
    int min = input_a->base < input_b->base ? input_a->base : input_b->base;
    int range = max - min + 1;
    
    // 避免除数为0
    if (range <= 0) {
        return 2; // 输入为空的时候,默认使用链表
    }
    
    int num = input_a->num + input_b->num;
    double sparsity = (double)num / range;
    
    // 如果平均稀疏度小于0.3，使用链表，否则使用数组
    return (sparsity < 0.3) ? 2 : 1;
}

/**
 * @brief 执行多项式操作的通用函数
 * @param input_a 第一个多项式的输入结构
 * @param input_b 第二个多项式的输入结构
 * @param operation_name 操作名称（用于显示）
 * @param arr_op 数组操作函数指针
 * @param list_op 链表操作函数指针
 */
static void perform_operation(input *input_a, input *input_b, const char *operation_name,
                      int* (*arr_op)(const int*, const int*),
                      poly* (*list_op)(poly*, poly*)) {

    if (!input_a || !input_b) {
        printf("操作失败：输入多项式为空\n");
        return;
    }

    // 根据两个多项式的稀疏性决定使用哪种数据结构
    int storage_type = determine_storage_type(input_a, input_b);
    
    if (storage_type == 1) { // 使用数组
        printf("基于稀疏度分析，使用数组存储进行%s运算\n", operation_name);
        int *poly_a = poly_arr_create(input_a);
        int *poly_b = poly_arr_create(input_b);
        
        if (!poly_a || !poly_b) {
            printf("操作失败：内存分配错误\n");
            if (poly_a) poly_arr_free(poly_a);
            if (poly_b) poly_arr_free(poly_b);
            return;
        }
        
        int *result = arr_op(poly_a, poly_b);
        
        printf("A %s B = ", operation_name);
        poly_arr_print(result);
        
        poly_arr_free(poly_a);
        poly_arr_free(poly_b);
        poly_arr_free(result);
    } else { // 使用链表
        printf("基于稀疏度分析，使用链表存储进行%s运算\n", operation_name);
        poly *poly_a = poly_list_create(input_a);
        poly *poly_b = poly_list_create(input_b);
        
        if (!poly_a || !poly_b) {
            printf("操作失败：内存分配错误\n");
            if (poly_a) poly_list_free(poly_a);
            if (poly_b) poly_list_free(poly_b);
            return;
        }
        
        poly *result = list_op(poly_a, poly_b);
        
        printf("A %s B = ", operation_name);
        Poly_list_print(result);
        printf("\n");
        
        poly_list_free(poly_a);
        poly_list_free(poly_b);
        poly_list_free(result);
    }
}

/**
 * @brief  从键盘中读取多项式的系数和指数, 系数为零的会被忽略,输入"0 0"接触`结束输入 
 * @param return 返回一个input结构体,包含系数和指数数组的指针,以及数量和指数的最值
 */
static input *poly_input()
{
    input *new_input = (input*)malloc(sizeof(input));
    if (!new_input)
        return NULL;
    
    array *array_xi = array_new(10);
    array *array_zhi = array_new(10);
    if (!array_xi || !array_zhi) {
        if (array_xi) array_destroy(array_xi);
        if (array_zhi) array_destroy(array_zhi);
        free(new_input);
        return NULL;
    }

    printf("输入多项式系数与指数,'0, 0'结束:\n");
    
    int max = INT_MIN, min = INT_MAX;
    int num = 0;
    int xi, zhi;
    for (int i = 0; ; i++){
        scanf("%d %d", &xi, &zhi);
        if (xi == 0 && zhi == 0) break;
        if (xi == 0) continue;
        if (zhi > max) max = zhi;
        if (zhi < min) min = zhi;
        array_append(array_xi, xi);
        array_append(array_zhi, zhi);
        num++;
    }
    
    new_input->xi = array_xi;
    new_input->zhi = array_zhi;
    new_input->base = min;
    new_input->max = max;
    new_input->num = num;
    
    return new_input;
}

/*---------------------------------下为接口函数-------------------*/

input *get_poly_input() {
    input *in = poly_input();
    if (!in) {
        printf("输入错误\n");
        return NULL;
    }
    return in;
}

void print_input(input* in)
{
    if (!in || in->num == 0) {
        printf("0");
        return;
    }
    
    int printed = 0;
    int xi, zhi;
    for (int i = 0; i < in->num; i++){
        array_get(in->xi, i, &xi);
        array_get(in->zhi, i, &zhi);
        
        if (printed){
            printf(xi > 0 ? " + " : " - ");
            xi = (xi > 0) ? xi : -xi;
        } else {
            if (xi < 0){
                printf("-"); // 移除前导空格
                xi = -xi;
            }
            printed = 1;
        }
        // 输出系数（如果系数不是1或指数为0）*重要*
        if (xi != 1 || zhi == 0){
            printf("%d", xi);
        }
        // 输出变量和指数
        if (zhi != 0){
            printf("x");
            if (zhi != 1)
                printf("^%d", zhi);
        }
    }
    
    // 处理所有系数都是0的情况
    if (!printed) {
        printf("0");
    }
}

void print_info(input *input_a, input *input_b)
{
    printf("多项式 A : ");
    print_input(input_a);
    printf("\n");
    printf("多项式 B : ");
    print_input(input_b);
    printf("\n");
}

void calculate_add(input *input_a, input *input_b) {
    perform_operation(input_a, input_b, "+", poly_arr_add, poly_list_add);
}

void calculate_neg(input *input_a, input *input_b) {
    perform_operation(input_a, input_b, "-", poly_arr_neg, poly_list_neg);
}

//乘法不判断稀疏性,直接默认使用链表
void calculate_mul(input *input_a, input *input_b) {
    if (!input_a || !input_b) {
        printf("操作失败：输入多项式为空\n");
        return;
    }
    
    poly *poly_a = poly_list_create(input_a);
    poly *poly_b = poly_list_create(input_b);
    
    if (!poly_a || !poly_b) {
        printf("操作失败：内存分配错误\n");
        if (poly_a) poly_list_free(poly_a);
        if (poly_b) poly_list_free(poly_b);
        return;
    }
    
    poly *result = poly_list_mul(poly_a, poly_b);
    
    printf("A * B = ");
    Poly_list_print(result);
    printf("\n");
    
    poly_list_free(poly_a);
    poly_list_free(poly_b);
    poly_list_free(result);
}