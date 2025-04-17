#include <stdio.h>
#include <stdlib.h>
#include "poly_arr.h"
#include "poly_list.h"
#include "input.h"

void print_menu() {
    printf("\n多项式计算器\n");
    printf("======================\n");
    printf("1. 输入多项式 A\n");
    printf("2. 输入多项式 B\n");
    printf("3. 计算 A + B\n");
    printf("4. 计算 A - B\n");
    printf("5. 计算 A * B\n");
    printf("6. 显示多项式 A\n");
    printf("7. 显示多项式 B\n");
    printf("0. 退出\n");
    printf("======================\n");
    printf("请选择操作: ");
}

int main() {
    int choice;
    int *poly_a_arr = NULL;
    int *poly_b_arr = NULL;
    int *result_arr = NULL;
    poly *poly_a_list = NULL;
    poly *poly_b_list = NULL;
    poly *result_list = NULL;
    int storage_type_a = 0; // 0:未定义 1:数组 2:链表
    int storage_type_b = 0; // 0:未定义 1:数组 2:链表
    
    while (1) {
        print_menu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                // 清理之前的多项式 A
                if (storage_type_a == 1 && poly_a_arr) {
                    poly_arr_free(poly_a_arr);
                    poly_a_arr = NULL;
                } else if (storage_type_a == 2 && poly_a_list) {
                    poly_free(poly_a_list);
                    poly_a_list = NULL;
                }
                
                // 输入新的多项式 A
                input *in_a = poly_input();
                if (in_a) {
                    if (in_a->flag == 2) { // 使用数组存储
                        poly_a_arr = poly_arr_create(in_a);
                        storage_type_a = 1;
                        printf("多项式 A 已采用数组方式存储\n");
                    } else { // 使用链表存储
                        poly_a_list = poly_list_create(in_a);
                        storage_type_a = 2;
                        printf("多项式 A 已采用链表方式存储\n");
                    }
                }
                break;
            }
            
            case 2: {
                // 清理之前的多项式 B
                if (storage_type_b == 1 && poly_b_arr) {
                    poly_arr_free(poly_b_arr);
                    poly_b_arr = NULL;
                } else if (storage_type_b == 2 && poly_b_list) {
                    poly_free(poly_b_list);
                    poly_b_list = NULL;
                }
                
                // 输入新的多项式 B
                input *in_b = poly_input();
                if (in_b) {
                    if (in_b->flag == 2) { // 使用数组存储
                        poly_b_arr = poly_arr_create(in_b);
                        storage_type_b = 1;
                        printf("多项式 B 已采用数组方式存储\n");
                    } else { // 使用链表存储
                        poly_b_list = poly_list_create(in_b);
                        storage_type_b = 2;
                        printf("多项式 B 已采用链表方式存储\n");
                    }
                }
                break;
            }
            
            case 3: { // A + B
                // 清理之前的结果
                if (result_arr) {
                    poly_arr_free(result_arr);
                    result_arr = NULL;
                }
                if (result_list) {
                    poly_free(result_list);
                    result_list = NULL;
                }
                
                printf("A + B = ");
                if (storage_type_a == 1 && storage_type_b == 1) {
                    result_arr = poly_arr_add(poly_a_arr, poly_b_arr);
                    poly_arr_print(result_arr);
                } else if (storage_type_a == 2 && storage_type_b == 2) {
                    result_list = poly_add(poly_a_list, poly_b_list);
                    Poly_list_print(result_list);
                    printf("\n");
                } else {
                    printf("错误：多项式 A 和 B 的存储方式不兼容\n");
                }
                break;
            }
            
            case 4: { // A - B
                // 清理之前的结果
                if (result_arr) {
                    poly_arr_free(result_arr);
                    result_arr = NULL;
                }
                if (result_list) {
                    poly_free(result_list);
                    result_list = NULL;
                }
                
                printf("A - B = ");
                if (storage_type_a == 1 && storage_type_b == 1) {
                    result_arr = poly_arr_neg(poly_a_arr, poly_b_arr);
                    poly_arr_print(result_arr);
                } else if (storage_type_a == 2 && storage_type_b == 2) {
                    result_list = poly_neg(poly_a_list, poly_b_list);
                    Poly_list_print(result_list);
                    printf("\n");
                } else {
                    printf("错误：多项式 A 和 B 的存储方式不兼容\n");
                }
                break;
            }
            
            case 5: { // A * B
                // 清理之前的结果
                if (result_arr) {
                    poly_arr_free(result_arr);
                    result_arr = NULL;
                }
                if (result_list) {
                    poly_free(result_list);
                    result_list = NULL;
                }
                
                printf("A * B = ");
                if (storage_type_a == 1 && storage_type_b == 1) {
                    result_arr = poly_arr_mul(poly_a_arr, poly_b_arr);
                    poly_arr_print(result_arr);
                } else if (storage_type_a == 2 && storage_type_b == 2) {
                    result_list = poly_mul(poly_a_list, poly_b_list);
                    Poly_list_print(result_list);
                    printf("\n");
                } else {
                    printf("错误：多项式 A 和 B 的存储方式不兼容\n");
                }
                break;
            }
            
            case 6: { // 显示多项式 A
                printf("多项式 A = ");
                if (storage_type_a == 1) {
                    poly_arr_print(poly_a_arr);
                } else if (storage_type_a == 2) {
                    Poly_list_print(poly_a_list);
                    printf("\n");
                } else {
                    printf("未定义\n");
                }
                break;
            }
            
            case 7: { // 显示多项式 B
                printf("多项式 B = ");
                if (storage_type_b == 1) {
                    poly_arr_print(poly_b_arr);
                } else if (storage_type_b == 2) {
                    Poly_list_print(poly_b_list);
                    printf("\n");
                } else {
                    printf("未定义\n");
                }
                break;
            }
            
            case 0: { // 退出
                // 清理所有资源
                if (poly_a_arr) poly_arr_free(poly_a_arr);
                if (poly_b_arr) poly_arr_free(poly_b_arr);
                if (result_arr) poly_arr_free(result_arr);
                if (poly_a_list) poly_free(poly_a_list);
                if (poly_b_list) poly_free(poly_b_list);
                if (result_list) poly_free(result_list);
                
                printf("感谢使用多项式计算器，再见！\n");
                return 0;
            }
            
            default:
                printf("无效的选项，请重新选择\n");
        }
    }
    
    return 0;
}