#include "poly_arr.h"
#include "input.h"

int *poly_arr_create(input *in)
{
    int size = in->max - in->base + 1;
    int base = 2 - in->base;
    int *poly_arr = (int*)malloc(sizeof(int) * (size + 2));
    if (!poly_arr) return NULL;

    memset(poly_arr,0,sizeof(int) * (size + 2));
    int xi,zhi;
    for (int i = 0; i < in->num; i++){
        array_get(in->xi, i, &xi);
        array_get(in->zhi, i, &zhi);
        poly_arr[zhi + base] = xi;
    }
    poly_arr[0] = size;
    poly_arr[1] = base;
    // array_destroy(in->xi);
    // array_destroy(in->zhi);
    // free(in);

    return poly_arr;
}

int *poly_arr_add(const int *a, const int *b)
{
    int a_min = 2 - a[1];
    int a_max = a[0] + a_min - 1;
    int b_min = 2 - b[1];
    int b_max = b[0] + b_min - 1;
    
    int min_zhi = (a_min < b_min) ? a_min : b_min;
    int max_zhi = (a_max > b_max) ? a_max : b_max;
    int size = max_zhi - min_zhi + 1;
    int base = 2 - min_zhi;

    int *result = (int*)malloc(sizeof(int) * (size + 2)); 
    if (!result) return NULL;

    memset(result, 0, sizeof(int) * (size + 2));
    result[0] = size;
    result[1] = base;
   
    for (int i = 0; i < a[0]; i++) {
        int zhi = i + a_min;
        int idx = zhi + base;
        if (idx >= 2 && idx < size + 2) {
            result[idx] += a[i + 2];
        }
    }
    
    for (int i = 0; i < b[0]; i++) {
        int zhi = i + b_min;
        int idx = zhi + base;
        if (idx >= 2 && idx < size + 2) {
            result[idx] += b[i + 2];
        }
    }
    
    return result;
}
int *poly_arr_neg(const int *a, const int *b)
{
    int a_min = 2 - a[1];
    int a_max = a[0] + a_min - 1;
    int b_min = 2 - b[1];
    int b_max = b[0] + b_min - 1;
    
    int min_zhi = (a_min < b_min) ? a_min : b_min;
    int max_zhi = (a_max > b_max) ? a_max : b_max;
    int size = max_zhi - min_zhi + 1;
    int base = 2 - min_zhi;

    int *result = (int*)malloc(sizeof(int) * (size + 2)); 
    if (!result) return NULL;

    memset(result, 0, sizeof(int) * (size + 2));
    result[0] = size;
    result[1] = base;
   
    for (int i = 0; i < a[0]; i++) {
        int zhi = i + a_min;
        int idx = zhi + base;
        if (idx >= 2 && idx < size + 2) {
            result[idx] += a[i + 2];
        }
    }
    
    for (int i = 0; i < b[0]; i++) {
        int zhi = i + b_min;
        int idx = zhi + base;
        if (idx >= 2 && idx < size + 2) {
            result[idx] -= b[i + 2];
        }
    }
    
    return result;
}

int *poly_arr_mul(const int *a, const int*b)
{
    int a_min = 2 - a[1];
    int a_max = a[0] + a_min - 1;
    int b_min = 2 - b[1];
    int b_max = b[0] + b_min - 1;
    
    int min_zhi = a_min + b_min;
    int max_zhi = a_max + b_max;
    int size = max_zhi - min_zhi + 1;
    int base = 2 - min_zhi;

    int *result = (int*)malloc(sizeof(int) * (size + 2)); 
    if (!result) return NULL;

    memset(result, 0, sizeof(int) * (size + 2));
    result[0] = size;
    result[1] = base;

    for (int i = 0; i < a[0]; i++){
        int a_zhi = i + a_min;
        int a_xi  = a[i + 2];
        for (int j = 0; j < b[0]; j++){
            int b_zhi = j + b_min;
            int b_xi  = b[j + 2];
            
            if (b_xi == 0) continue;

            int res_zhi = a_zhi + b_zhi;
            int res_xi  = a_xi * b_xi;

            int res_idx = res_zhi + base;

            result[res_idx] += res_xi;
        }
    }

    return result;
}

void poly_arr_print(const int *poly)
{
    if (!poly) {
        printf("0\n");
        return;
    }
    
    int size = poly[0];
    int base = poly[1];
    int min_exp = 2 - base;
    int printed = 0;
    
    for (int i = size - 1; i >= 0; i--) {
        int exp = min_exp + i;
        int coef = poly[i + 2];
        
        if (coef != 0) {
            // 处理符号
            if (printed) {
                printf(coef > 0 ? " + " : " - ");
                coef = (coef > 0) ? coef : -coef;  // 取绝对值
            } else {
                if (coef < 0) {
                    printf("-");
                    coef = -coef;  // 取绝对值
                }
                printed = 1;
            }
            
            // 打印系数（除非系数为1且指数不为0）
            if (coef != 1 || exp == 0) {
                printf("%d", coef);
            }
            
            // 打印变量和指数
            if (exp != 0) {
                printf("x");
                if (exp != 1) {
                    printf("^%d", exp);
                }
            }
        }
    }
    
    // 如果多项式为零
    if (!printed) {
        printf("0");
    }
    
    printf("\n");
}
void poly_arr_free(int *poly)
{
    if (poly) free(poly);
}