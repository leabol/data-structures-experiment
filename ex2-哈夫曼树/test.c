#include "read_file.h"
#include "pri_queue.h"
#include "sort.h"
#include <stdlib.h>
#include <stdio.h>

// 递归获取每个叶子节点的值和深度
void get_nodes_lens(Node* root, int depth, int *lens){
    // 如果是叶子节点（没有左右子节点）
    if (root->left == NULL && root->right == NULL) {
        lens[root->symbol] = depth;
        return;
    }
    
    // 左子树路径编码为0
    if (root->left) {
       get_nodes_lens (root->left, depth + 1, lens);
    }
    
    // 右子树路径编码为1
    if (root->right) {
        get_nodes_lens(root->right, depth + 1, lens);
    }
}

// 打印哈夫曼树所有叶子节点的值和深度
// void print(Node *head) {
//     if (head == NULL) {
//         printf("空树\n");
//         return;
//     }
    
//     // 假设最大编码长度不超过100
//     int code[100] = {0};
//     get_leaf_nodes(head, 0, code, 0);
// }

int char_num[MAX_UTF8_NUM];
int main()
{
    read_text("text1", char_num);

    transfer_to_utf8(char_num);

    Heap *head = create_heap(head);

    for (int i = 0; i < MAX_UTF8_NUM; i++){
        if (char_num[i] == 0) continue;
        
        Node *new_node = (Node*)malloc(sizeof(Node));
        new_node->num = char_num[i];
        new_node->symbol = i;
        new_node->left = NULL;
        new_node->right = NULL;
        
        push(head, new_node);
    }
    int node_num = size(head);

    while (size(head) > 1){
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
    
    
    int lens[MAX_UTF8_NUM] = {0};

    if (!empty(head)) {
        Node root = top(head);
        printf("哈夫曼树的所有叶子节点:\n");
        get_nodes_lens(&root, 0, lens);
    } else {
        printf("构建哈夫曼树失败\n");
    }

    int **order_lens = (int**)malloc(node_num * sizeof(int*)); // 分配行指针
    order_lens[0] = (int*)malloc(node_num * 2 * sizeof(int)); // 分配连续内存块
    for (int i = 1; i < node_num; i++) {
        order_lens[i] = order_lens[0] + i * 2; // 每行起始地址
    }
    int index = 0;
    for (int i = 0; i < MAX_UTF8_NUM; i++){
        if (lens[i] != 0){
            order_lens[index][0] = i;
            order_lens[index++][1] = lens[i];
        }
    }

    sort(order_lens, 0, node_num - 1);

    for (int i = 0; i < node_num; i++){
        if (order_lens[i] != 0)
            printf("%d : %d\n", order_lens[i][0], order_lens[i][1]);
    }
    return 0;
}