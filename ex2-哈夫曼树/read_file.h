/**
 * @file read_file.h
 * @brief UTF-8文本文件读取和处理功能
 * 
 * 该文件提供了用于读取UTF-8编码文本文件并统计字符出现频率的函数
 */
#include <stdio.h>
#ifndef READ_FILE_H
#define READ_FILE_H

#define MAX_UTF8_NUM 650000  // UTF-8编码中可能出现的最大编码值

/**
 * @brief 读取文本文件并统计各字符出现次数
 * 
 * 该函数读取指定路径的UTF-8编码文本文件，
 * 解析UTF-8编码并统计每个字符的出现次数
 * 
 * @param text_path 文本文件路径
 * @param char_num 存储字符出现次数的数组
 * @return 成功返回0，失败返回非0值
 */
int read_text(const char *text_path, int *char_num);

/**
 * @brief 将UTF-8编码转换并打印字符统计结果
 * 
 * 该函数遍历字符数组，将有出现次数的字符转换为UTF-8编码格式并打印
 * 统计信息，同时输出总字符数和不同字符数的统计结果
 * 
 * @param char_num 存储字符出现次数的数组
 */
void transfer_to_utf8(int *char_num);


int read_char(FILE *fp);
void to_utf8(int decode, char *utf_8);
#endif /* READ_FILE_H */