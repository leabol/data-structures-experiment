#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/sort.h"

// 处理用户选择
void processChoice(int choice, SqList *L) {
    SqList tempL;
    Counter counter;
    clock_t start, end;
    double cpu_time_used;
    char *algorithmName;
    
    if (choice == 0) {
        return;
    }
    
    // 复制一个列表用于排序
    memcpy(&tempL, L, sizeof(SqList));
    resetCounter(&counter);
    
    switch (choice) {
        case 1:
            algorithmName = "简单选择排序";
            break;
        case 2:
            algorithmName = "直接插入排序";
            break;
        case 3:
            algorithmName = "冒泡排序";
            break;
        case 4:
            algorithmName = "折半插入排序";
            break;
        case 5:
            algorithmName = "希尔排序";
            break;
        case 6:
            algorithmName = "快速排序(递归版本)";
            break;
        case 7:
            algorithmName = "快速排序(非递归版本)";
            break;
        case 8:
            algorithmName = "堆排序";
            break;
        default:
            printf("无效的选择!\n");
            return;
    }
    
    printf("======== %s ========\n", algorithmName);
    printf("排序前: ");
    printList(&tempL, "");
    printf("\n");
    
    // 记录开始时间
    start = clock();
    
    // 执行排序算法
    switch (choice) {
        case 1:
            SelectSort(&tempL, &counter);
            break;
        case 2:
            InsertSort(&tempL, &counter);
            break;
        case 3:
            BubbleSort(&tempL, &counter);
            break;
        case 4:
            BinaryInsertSort(&tempL, &counter);
            break;
        case 5:
            ShellSort(&tempL, &counter);
            break;
        case 6:
            QuickSort(&tempL, &counter);
            break;
        case 7:
            QuickSort_NonRecursive(&tempL, &counter);
            break;
        case 8:
            HeapSort(&tempL, &counter);
            break;
    }
    
    // 记录结束时间
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("排序后: ");
    printList(&tempL, "");
    printf("\n");
    
    printf("性能统计: 比较%d次, 移动%d次, 用时%.6f秒, %s\n", 
           counter.compares, counter.moves, cpu_time_used,
           checkStability(&tempL) ? "稳定" : "不稳定");
}

int main() {
    SqList L;
    int i;
    
    while(1){
        // 手动输入数据
        inputTestData(&L);
        
        printf("所有排序算法将依次执行，请稍候...\n\n");
        
        // 依次执行所有排序算法
        for (i = 1; i <= 8; i++) {
            processChoice(i, &L);
            printf("\n");
        }
    } 
    return 0;
}
