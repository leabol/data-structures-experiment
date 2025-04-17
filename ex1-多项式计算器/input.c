#include "input.h"

input *poly_input()
{
    input *new_input = (input*)malloc(sizeof(input));
    if (!new_input)
        return NULL;
    
    array *array_xi = array_new(10);
    array *array_zhi = array_new(10);

    printf("输入多项式系数与指数,'0, 0'结束:\n");
    new_input->xi = array_xi;
    new_input->zhi = array_zhi;
    new_input->flag = 0;
    new_input->base = 0;

    int max = -1000000000, min = 1000000000;
    int num = 0;
    int xi, zhi;
    for (int i = 0; ; i++){
        scanf("%d %d", &xi, &zhi);
        if (xi == 0 && zhi == 0) break;
        if (zhi > max) max = zhi;
        if (zhi < min) min = zhi;
        array_append(array_xi,xi);
        array_append(array_zhi,zhi);
        num++;
    }

    new_input->base = min;
    new_input->max = max;
    if (num/(max-min + 1) < 0.3) new_input->flag = 1;//1 使用链表
    else new_input->flag = 2;//使用数组

    return new_input;
}