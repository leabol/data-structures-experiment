#include "read_file.h"
#include "pri_queue.h"
#include "sort.h"
#include "huffman.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_PATH_LENGTH 256

int char_num[MAX_UTF8_NUM];

int main(int argc, char *argv[]) {
    // 文件路径缓冲区
    char inputFile[MAX_PATH_LENGTH];
    
    const char* encodedFile = "Text/encoded_file";
    const char* decodedFile = "Text/decoded_file";
    
    printf("=== 哈夫曼编码解码系统 ===\n");
    
    printf("请输入文件路径: ");
    scanf(" %s", inputFile);
    
    printf("\n将使用以下文件:\n");
    printf("原始文件: %s\n", inputFile);
    printf("编码文件: %s\n", encodedFile);
    printf("解码文件: %s\n", decodedFile);
    
    printf("\n正在读取文本并统计频率...\n");
    
    // 1. 读取文本并统计字符频率
    if (read_text(inputFile, char_num) != 0) {
        printf("无法读取输入文件: %s\n", inputFile);
        return 1;
    }
    
    // 2.构建优先队列 
    Heap *head = create_heap(head);
    
    // 根据字符频率创建初始节点并放入优先队列
    for (int i = 0; i < MAX_UTF8_NUM; i++) {
        if (char_num[i] == 0) continue;
        
        Node *new_node = (Node*)malloc(sizeof(Node));//哈夫曼节点
        if (!new_node){
            free_heap(head);
            return 1;
        }

        new_node->num = char_num[i];
        new_node->symbol = i;
        new_node->left = NULL;
        new_node->right = NULL;
        
        push(head, new_node);
    }
    int node_num = size(head);
    printf("共有不同字符 %d 个\n", node_num);
    
    // 3. 构建哈夫曼树
    printf("正在构建哈夫曼树...\n");
    while (size(head) > 1) {
        Node x = top(head);
        pop(head);
        Node y = top(head);
        pop(head);
        
        Node *parent = (Node*)malloc(sizeof(Node));
        parent->symbol = '\0';
        parent->num = x.num + y.num;
        
        // 创建新节点来保存从堆中取出的节点数据
        Node *left_child = (Node*)malloc(sizeof(Node));
        *left_child = x;
        Node *right_child = (Node*)malloc(sizeof(Node));
        *right_child = y;
        
        parent->left = left_child;
        parent->right = right_child;
        
        push(head, parent);
    }
    
    // 4. 获取编码长度
    int lens[MAX_UTF8_NUM] = {0};
    if (!empty(head)) {
        Node root = top(head);
        getNodesLens(&root, 0, lens);
    }
    
    // 5. 按编码长度排序
    printf("正在生成规范编码...\n");
    int **order_lens = (int**)malloc(node_num * sizeof(int*)); // 分配行指针
    order_lens[0] = (int*)malloc(node_num * 2 * sizeof(int)); // 分配连续内存块
    for (int i = 1; i < node_num; i++) {
        order_lens[i] = order_lens[0] + i * 2; // 每行起始地址
    }
    /*
    *[0] -> [][][][][][][]....[][][]
    *[1]--------^   ^
    *[1]------------|
    *[2]
    * 
    */

    int index = 0;
    for (int i = 0; i < MAX_UTF8_NUM; i++) {
        if (lens[i] != 0) {
            order_lens[index][0] = i;//符号
            order_lens[index++][1] = lens[i];//字频
        }
    }
    sort(order_lens, node_num);

    // 6. 生成规范哈夫曼编码
    int code[MAX_UTF8_NUM][2] = {0};
    int code_tmp = 0;
    int code_len = order_lens[0][1];
    int first_symbol = order_lens[0][0];
    code[first_symbol][0] = code_tmp;
    code[first_symbol][1] = code_len;
    
    for (int i = 1; i < node_num; i++) {
        if (code_len != order_lens[i][1]){//编码不同的在编码加以后,左移k位
            int k = order_lens[i][1] - code_len;
            code_len = order_lens[i][1];
            code_tmp = ((code_tmp + 1) << k);
        } else {
            code_tmp++;//编码长度相同的, 编码加一
        }
        code[order_lens[i][0]][0] = code_tmp;
        code[order_lens[i][0]][1] = code_len;
    }
    
    // 7. 编码
    printf("正在执行编码...\n");
    int encodeResult = huffmanEncode(inputFile, encodedFile, code);
    if (encodeResult == 0) {
        printf("编码完成，已保存到文件：%s\n", encodedFile);
    } else {
        printf("编码失败\n");
        free(order_lens[0]);
        free(order_lens);
        return 1;
    }
    
    // 8. 解码
    printf("正在执行解码...\n");
    int decodeResult = huffmanDecode(encodedFile, decodedFile, code, node_num);
    if (decodeResult == 0) {
        printf("解码完成，已保存到文件：%s\n", decodedFile);
    } else {
        printf("解码失败\n");
        free(order_lens[0]);
        free(order_lens);
        return 1;
    }
    
    // 9. 比较原始文件和解码文件
    printf("正在比较原始文件和解码文件...\n");
    compareFiles(inputFile, decodedFile);
    
    // 10. 释放内存
    free(order_lens[0]);
    free(order_lens);
    
    printf("=== 编解码过程结束 ===\n");
    return 0;
}