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
    printf("0. 退出\n");
    printf("======================\n");
    printf("请选择操作: ");
}

int main()
{
    int choice;
    input *input_a = NULL;
    input *input_b = NULL;
    int *poly_a_arr = NULL;
    int *poly_b_arr = NULL;
    poly *poly_a_list = NULL;
    poly *poly_b_list = NULL;

    while (1){
        print_menu();
        scanf("%d", &choice);

        switch (choice){
            case 1:
            { // 输入多项式 A
                // 清理之前的多项式 A
                if (poly_a_arr){
                    poly_arr_free(poly_a_arr);
                    poly_a_arr = NULL;
                }
                if (poly_a_list){
                    poly_list_free(poly_a_list);
                    poly_a_list = NULL;
                }
                // 释放先前的input_a
                if (input_a){
                    // 先检查并释放数组
                    if (input_a->xi)
                        array_destroy(input_a->xi);
                    if (input_a->zhi)
                        array_destroy(input_a->zhi);
                    // 然后释放结构体本身
                    free(input_a);
                    input_a = NULL;
                }
                // 获取新的多项式 A
                input_a = get_poly_input();
                if (input_a){
                    printf("多项式 A 已输入: ");
                    print_input(input_a);
                    printf("\n");
                }
                break;
            }

            case 2:
            { 
                if (poly_b_arr){
                    poly_arr_free(poly_b_arr);
                    poly_b_arr = NULL;
                }
                if (poly_b_list){
                    poly_list_free(poly_b_list);
                    poly_b_list = NULL;
                }
                if (input_b){
                    if (input_b->xi)
                        array_destroy(input_b->xi);
                    if (input_b->zhi)
                        array_destroy(input_b->zhi);
                    free(input_b);
                    input_b = NULL;
                }

                input_b = get_poly_input();
                if (input_b){
                    printf("多项式 B 已输入: ");
                    print_input(input_b);
                    printf("\n");
                }
                break;
            }

            case 3:
            { // A + B
                if (!input_a || !input_b){
                    printf("请先输入多项式 A 和 B\n");
                    break;
                }
                print_info(input_a,input_b);
                calculate_add(input_a, input_b);
                break;
            }

            case 4:
            { // A - B
                if (!input_a || !input_b){
                    printf("请先输入多项式 A 和 B\n");
                    break;
                }
                print_info(input_a,input_b);
                calculate_neg(input_a, input_b);
                break;
            }

            case 5:
            { // A * B
                if (!input_a || !input_b){
                    printf("请先输入多项式 A 和 B\n");
                    break;
                }
                print_info(input_a,input_b);
                calculate_mul(input_a, input_b);
                break;
            }

            case 0:
            { // 退出
                // 清理所有资源
                if (poly_a_arr)
                    poly_arr_free(poly_a_arr);
                if (poly_b_arr)
                    poly_arr_free(poly_b_arr);
                if (poly_a_list)
                    poly_list_free(poly_a_list);
                if (poly_b_list)
                    poly_list_free(poly_b_list);

                // 释放input_a
                if (input_a){
                    if (input_a->xi)
                        array_destroy(input_a->xi);
                    if (input_a->zhi)
                        array_destroy(input_a->zhi);
                    free(input_a);
                }

                // 释放input_b
                if (input_b){
                    if (input_b->xi)
                        array_destroy(input_b->xi);
                    if (input_b->zhi)
                        array_destroy(input_b->zhi);
                    free(input_b);
                }
                return 0;
            }

            default:
                printf("无效的选项，请重新选择\n");
            }
    }
}