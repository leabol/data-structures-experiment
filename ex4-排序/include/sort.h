#ifndef SORT_H
#define SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 100 /*参加排序元素的最大个数*/

typedef int KeyType;

typedef struct {
    char name[20]; // 一个简单的名称字段作为其他信息
    int index;     // 原始位置，用于判断排序算法的稳定性
} InfoType;

typedef struct {
    KeyType key;
    InfoType otherinfo;
} RedType;

typedef struct {
    RedType r[MAXSIZE+1];  // r[0]用作哨兵或临时变量
    int length;            // 参加排序元素的实际个数
} SqList;

// 计数器结构，用于记录比较次数和移动次数
typedef struct {
    int compares;  // 比较次数
    int moves;     // 移动次数
} Counter;

// 辅助函数
void swap(RedType *a, RedType *b, Counter *counter);
void printList(SqList *L, const char *message);
void resetCounter(Counter *counter);

// 简单选择排序
void SelectSort(SqList *L, Counter *counter);

// 直接插入排序
void InsertSort(SqList *L, Counter *counter);

// 冒泡排序
void BubbleSort(SqList *L, Counter *counter);

// 折半插入排序
void BinaryInsertSort(SqList *L, Counter *counter);

// 希尔排序
void ShellSort(SqList *L, Counter *counter);

// 快速排序（递归版本）
void QuickSort_Recursive(SqList *L, int low, int high, Counter *counter);
void QuickSort(SqList *L, Counter *counter);

// 快速排序（非递归版本）
void QuickSort_NonRecursive(SqList *L, Counter *counter);

// 堆排序
void HeapSort(SqList *L, Counter *counter);

// 判断排序算法的稳定性
int checkStability(SqList *L);

// 菜单和主程序函数
void processChoice(int choice, SqList *L);
void inputTestData(SqList *L);

#endif // SORT_H
