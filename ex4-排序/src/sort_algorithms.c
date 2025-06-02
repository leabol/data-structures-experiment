#include "../include/sort.h"

// 交换两个元素
void swap(RedType *a, RedType *b, Counter *counter) {
    RedType temp = *a;
    *a = *b;
    *b = temp;
    counter->moves += 3;  // 一次交换包含3次移动操作
}

// 打印列表内容
void printList(SqList *L, const char *message) {
    printf("%s [", message);
    for (int i = 1; i <= L->length; i++) {
        printf("%d", L->r[i].key);
        if (i < L->length) {
            printf(", ");
        }
    }
    printf("]");
}

// 重置计数器
void resetCounter(Counter *counter) {
    counter->compares = 0;
    counter->moves = 0;
}

// 简单选择排序
void SelectSort(SqList *L, Counter *counter) {
    int i, j, min;
    
    for (i = 1; i <= L->length - 1; i++) {
        min = i;
        for (j = i + 1; j <= L->length; j++) {
            counter->compares++;
            if (L->r[j].key < L->r[min].key) {
                min = j;
            }
        }
        if (min != i) {
            swap(&L->r[i], &L->r[min], counter);
        }
    }
}

// 直接插入排序
void InsertSort(SqList *L, Counter *counter) {
    int i, j;
    for (i = 2; i <= L->length; i++) {
        L->r[0] = L->r[i];  // r[0]作为哨兵
        counter->moves++;
        j = i - 1;
        counter->compares++;
        while (L->r[0].key < L->r[j].key) {
            L->r[j + 1] = L->r[j];
            counter->moves++;
            j--;
            counter->compares++;
        }
        L->r[j + 1] = L->r[0];
        counter->moves++;
    }
}

// 冒泡排序
void BubbleSort(SqList *L, Counter *counter) {
    int i, j;
    int flag;
    
    for (i = 1; i <= L->length - 1; i++) {
        flag = 0;
        for (j = 1; j <= L->length - i; j++) {
            counter->compares++;
            if (L->r[j].key > L->r[j + 1].key) {
                swap(&L->r[j], &L->r[j + 1], counter);
                flag = 1;
            }
        }
        
        if (flag == 0) {
            break;  // 如果这一轮没有发生交换，则已经排好序
        }
    }
}

// 折半插入排序
void BinaryInsertSort(SqList *L, Counter *counter) {
    int i, j, low, high, mid;
    
    for (i = 2; i <= L->length; i++) {
        L->r[0] = L->r[i];  // r[0]作为哨兵
        counter->moves++;
        
        low = 1;
        high = i - 1;
        while (low <= high) {
            mid = (low + high) / 2;
            counter->compares++;
            if (L->r[0].key < L->r[mid].key) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        
        for (j = i - 1; j >= high + 1; j--) {
            L->r[j + 1] = L->r[j];
            counter->moves++;
        }
        
        L->r[high + 1] = L->r[0];
        counter->moves++;
    }
}

// 希尔排序
void ShellSort(SqList *L, Counter *counter) {
    int i, j, increment;
    
    for (increment = L->length / 2; increment > 0; increment /= 2) {
        for (i = increment + 1; i <= L->length; i++) {
            if (L->r[i].key < L->r[i - increment].key) {
                counter->compares++;
                L->r[0] = L->r[i];  // r[0]作为暂存单元
                counter->moves++;
                
                for (j = i - increment; j > 0 && L->r[0].key < L->r[j].key; j -= increment) {
                    counter->compares++;
                    L->r[j + increment] = L->r[j];
                    counter->moves++;
                }
                
                L->r[j + increment] = L->r[0];
                counter->moves++;
            }
        }
    }
}

// 快速排序辅助函数 - 分区
int Partition(SqList *L, int low, int high, Counter *counter) {
    int pivotkey = L->r[low].key;  // 选取第一个元素作为枢轴
    counter->moves++;
    
    L->r[0] = L->r[low];  // 用r[0]存储枢轴值
    counter->moves++;
    
    while (low < high) {
        while (low < high && L->r[high].key >= pivotkey) {
            counter->compares++;
            high--;
        }
        L->r[low] = L->r[high];
        counter->moves++;
        
        while (low < high && L->r[low].key <= pivotkey) {
            counter->compares++;
            low++;
        }
        L->r[high] = L->r[low];
        counter->moves++;
    }
    
    L->r[low] = L->r[0];  // 将枢轴值放回最终位置
    counter->moves++;
    
    return low;
}

// 快速排序递归版本
void QuickSort_Recursive(SqList *L, int low, int high, Counter *counter) {
    int pivot;
    if (low < high) {
        pivot = Partition(L, low, high, counter);
        
        QuickSort_Recursive(L, low, pivot - 1, counter);
        QuickSort_Recursive(L, pivot + 1, high, counter);
    }
}

void QuickSort(SqList *L, Counter *counter) {
    QuickSort_Recursive(L, 1, L->length, counter);
}

// 快速排序非递归版本
typedef struct {
    int low;
    int high;
} Stack;

void QuickSort_NonRecursive(SqList *L, Counter *counter) {
    Stack stack[MAXSIZE];
    int top = -1;
    int pivot;
    int low, high;
    
    // 初始状态入栈
    stack[++top].low = 1;
    stack[top].high = L->length;
    
    while (top >= 0) {
        // 出栈
        low = stack[top].low;
        high = stack[top--].high;
        
        if (low < high) {
            pivot = Partition(L, low, high, counter);
            
            // 右边序列入栈
            stack[++top].low = pivot + 1;
            stack[top].high = high;
            
            // 左边序列入栈
            stack[++top].low = low;
            stack[top].high = pivot - 1;
        }
    }
}

// 堆排序辅助函数 - 向下调整
void HeapAdjust(SqList *L, int s, int m, Counter *counter) {
    int j;
    RedType temp = L->r[s];
    counter->moves++;
    
    for (j = 2 * s; j <= m; j *= 2) {
        if (j < m && L->r[j].key < L->r[j + 1].key) {
            counter->compares++;
            j++;
        }
        
        counter->compares++;
        if (temp.key >= L->r[j].key) {
            break;
        }
        
        L->r[s] = L->r[j];
        counter->moves++;
        s = j;
    }
    
    L->r[s] = temp;
    counter->moves++;
}

// 堆排序
void HeapSort(SqList *L, Counter *counter) {
    int i;
    
    // 构建初始堆
    for (i = L->length / 2; i > 0; i--) {
        HeapAdjust(L, i, L->length, counter);
    }
    
    // 进行n-1次向下调整
    for (i = L->length; i > 1; i--) {
        swap(&L->r[1], &L->r[i], counter);  // 将堆顶元素与最后一个元素交换
        HeapAdjust(L, 1, i - 1, counter);   // 将剩余的i-1个元素重新调整为大顶堆
    }
}

// 判断排序算法的稳定性
int checkStability(SqList *L) {
    int i;
    for (i = 1; i < L->length; i++) {
        if (L->r[i].key == L->r[i + 1].key && 
            L->r[i].otherinfo.index > L->r[i + 1].otherinfo.index) {
            return 0;  // 不稳定
        }
    }
    return 1;  // 稳定
}

// 手动输入测试数据
void inputTestData(SqList *L) {
    int i, n;
    
    printf("===== 输入测试数据 =====\n");
    printf("数据数量 (1-%d): ", MAXSIZE);
    
    while (scanf("%d", &n) != 1 || n <= 0 || n > MAXSIZE) {
        while (getchar() != '\n');
        printf("请输入1到%d之间的整数: ", MAXSIZE);
    }
    
    L->length = n;
    printf("请输入%d个整数:\n", n);
    
    for (i = 1; i <= n; i++) {
        printf("数据 %d: ", i);
        while (scanf("%d", &(L->r[i].key)) != 1) {
            while (getchar() != '\n');
            printf("请输入一个整数: ");
        }
        // 保存原始位置用于判断稳定性
        sprintf(L->r[i].otherinfo.name, "Item_%d", L->r[i].key);
        L->r[i].otherinfo.index = i;
    }
    
    printf("\n输入完成! ");
    printList(L, "您输入的数据是");
    printf("\n");
}
