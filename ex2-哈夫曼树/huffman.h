/**
 * @file huffman.h
 * @brief 哈夫曼树编码和解码功能
 * 
 * 该文件定义了哈夫曼树的数据结构及对其进行的操作，包括构建哈夫曼树、编码和解码
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pri_queue.h"

/**
 * @brief 构建哈夫曼解码树
 * 
 * 根据给定的编码表构建哈夫曼解码树，用于后续解码操作
 * 
 * @param code 编码表，code[i][0]存储字符i的编码，code[i][1]存储其长度
 * @param charCount 字符数量
 * @return 哈夫曼树的根节点
 */
Node* buildHuffmanTree(int code[][2], int charCount);

/**
 * @brief 释放哈夫曼树占用的内存
 * 
 * 递归释放哈夫曼树的所有节点
 * 
 * @param node 开始释放的节点，通常是根节点
 */
void freeHuffmanTree(Node* node);

/**
 * @brief 获取哈夫曼树中每个字符的编码长度
 * 
 * 递归遍历哈夫曼树，获取每个叶子节点的深度（编码长度）
 * 
 * @param root 哈夫曼树的根节点
 * @param depth 当前节点的深度
 * @param lens 存储各字符编码长度的数组
 */
void getNodesLens(Node* root, int depth, int *lens);

/**
 * @brief 执行哈夫曼编码并写入文件
 * 
 * 读取源文件，根据哈夫曼编码表进行编码，并写入目标文件
 * 
 * @param sourcePath 源文件路径
 * @param targetPath 目标文件路径
 * @param code 哈夫曼编码表
 * @return 成功返回0，失败返回非0值
 */
int huffmanEncode(const char* sourcePath, const char* targetPath, int code[][2]);

/**
 * @brief 执行哈夫曼解码并写入文件
 * 
 * 读取编码后的文件，根据哈夫曼树进行解码，并写入目标文件
 * 
 * @param sourcePath 编码文件路径
 * @param targetPath 解码后的目标文件路径
 * @param code 哈夫曼编码表
 * @param charCount 字符数量
 * @return 成功返回0，失败返回非0值
 */
int huffmanDecode(const char* sourcePath, const char* targetPath, int code[][2], int charCount);

/**
 * @brief 比较两个文件的内容
 * 
 * 逐字节比较两个文件的内容，输出匹配情况和差异统计
 * 
 * @param file1Path 第一个文件的路径
 * @param file2Path 第二个文件的路径
 * @return 成功返回0，失败返回非0值；如果文件完全匹配返回0，不匹配返回1
 */
int compareFiles(const char* file1Path, const char* file2Path);

/**
 * @brief 打印二进制数的位表示
 * 
 * 用于调试时查看二进制编码
 * 
 * @param num 要打印的数字
 * @param bits 要打印的位数
 */
void printBinary(int num, int bits);

#endif /* HUFFMAN_H */