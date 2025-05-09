#include "huffman.h"
#include "read_file.h"
#include <string.h>

Node* buildHuffmanTree(int code[][2], int charCount) {
    Node* root = (Node*)malloc(sizeof(Node));
    root->left = NULL;
    root->right = NULL;
    root->symbol = -1; 

    for (int i = 0; i < MAX_UTF8_NUM; i++) {
        if (code[i][1] == 0) continue; 
        
        int huffCode = code[i][0];
        int codeLen = code[i][1];
        
        Node* current = root;
        for (int j = codeLen - 1; j >= 0; j--) {
            int bit = (huffCode >> j) & 1;
            
            if (bit == 0) {
                if (!current->left) {
                    current->left = (Node*)malloc(sizeof(Node));
                    current->left->left = NULL;
                    current->left->right = NULL;
                    current->left->symbol = -1;
                }
                current = current->left;
            } else {
                if (!current->right) {
                    current->right = (Node*)malloc(sizeof(Node));
                    current->right->left = NULL;
                    current->right->right = NULL;
                    current->right->symbol = -1;
                }
                current = current->right;
            }
            
            if (j == 0) {
                current->symbol = i;
            }
        }
    }
    return root;
}

void freeHuffmanTree(Node* node) {
    if (node == NULL) return;
    freeHuffmanTree(node->left);
    freeHuffmanTree(node->right);
    free(node);
}

void printBinary(int num, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        // 从高位到低位逐位提取并打印
        printf("%c", (num >> i) & 1 ? '1' : '0');
        if (i % 4 == 0 && i != 0) printf(" ");
    }
    printf("\n");
}

void getNodesLens(Node* root, int depth, int *lens)
{
    if (root->left == NULL && root->right == NULL){
        lens[root->symbol] = depth;
        return;
    }
    
    if (root->left){
       getNodesLens(root->left, depth + 1, lens);
    }
    
    if (root->right){
        getNodesLens(root->right, depth + 1, lens);
    }
}

int huffmanEncode(const char* sourcePath, const char* targetPath, int code[][2])
{
    int ch;
    FILE *write_fp;
    if ((write_fp = fopen(targetPath, "wb+")) == NULL){
        printf("Failed to open target file: %s\n", targetPath);
        return 1;
    }

    FILE *fp;
    if ((fp = fopen(sourcePath, "rb")) == NULL){
        printf("Failed to open source file: %s\n", sourcePath);
        fclose(write_fp);
        return 1;
    }
    
    char buff = 0;
    int buff_size = 8;//buff剩余的位数
    long long byte_size = 0;
    
    // 写入字节大小信息到文件头部
    fwrite(&byte_size, sizeof(long long), 1, write_fp);
    int padding_pos = ftell(write_fp);  // 记录填充位信息的位置
    unsigned char padding = 0;          // 最后一个字节的填充位数
    fwrite(&padding, sizeof(unsigned char), 1, write_fp);

    // 编码开始 
    while ((ch = read_char(fp)) != -1){
        int hafcode = code[ch][0];
        int code_len = code[ch][1];
        byte_size++;
        
        int mask = (1 << (code_len - 1));
        while (code_len--){
            if (hafcode & mask) //获取hafcode最高位
                buff |= 1;
            
            mask >>= 1;
            buff_size--;
            if (buff_size == 0){
                fputc(buff, write_fp);
                buff_size = 8;
                buff = 0;
            }
            buff <<= 1;
        } 
    }
    
    // 处理最后不足8位的情况
    if (buff_size != 8){
        buff <<= (buff_size - 1);//将剩余的位以到最高位写入
        fputc(buff, write_fp);
        padding = buff_size;  // 记录填充位数
    }
    
    // 回写字节大小和填充位信息
    fseek(write_fp, 0, SEEK_SET);
    fwrite(&byte_size, sizeof(long long), 1, write_fp);
    fseek(write_fp, padding_pos, SEEK_SET);
    fwrite(&padding, sizeof(unsigned char), 1, write_fp);

    fclose(write_fp);
    fclose(fp);
    
    return 0;
}

int huffmanDecode(const char* sourcePath, const char* targetPath, int code[][2], int charCount) {
    FILE *source_fp;
    if ((source_fp = fopen(sourcePath, "rb")) == NULL){
        printf("Failed to open source file: %s\n", sourcePath);
        return 1;
    }
    
    FILE *decode_fp;
    if ((decode_fp = fopen(targetPath, "wb+")) == NULL){
        printf("Failed to open target file: %s\n", targetPath);
        fclose(source_fp);
        return 1;
    }

    long long expected_byte_size = 0;//应该读取的字节数
    unsigned char padding_bits = 0;//末尾填充的位数
    fread(&expected_byte_size, sizeof(long long), 1, source_fp);
    fread(&padding_bits, sizeof(unsigned char), 1, source_fp);
    
    printf("文件信息: 字符数=%lld, 填充位数=%d\n", expected_byte_size, padding_bits);

    Node *huffmanTree = buildHuffmanTree(code, charCount);
    unsigned char readByte;
    Node *current = huffmanTree;
    int bitPos = 7; // 缓冲区剩余可读位数
    int bytesRead = 0;//从编码文件中,读取的字节数
    int charsWritten = 0;//写入解码文件的字节数

    long long fileSize = 0;//编码文件真实大小
    fseek(source_fp, 0, SEEK_END);
    fileSize = ftell(source_fp);
    
    fseek(source_fp, sizeof(long long) + sizeof(unsigned char), SEEK_SET);

    if (fread(&readByte, 1, 1, source_fp) != 1) {
        printf("文件为空\n");
        freeHuffmanTree(huffmanTree);
        fclose(source_fp);
        fclose(decode_fp);
        return 1;
    } 
    
    bytesRead++;//读取字符加一
    
    while (bytesRead <= fileSize - sizeof(long long) - sizeof(unsigned char)){
        // 独到结尾，退出循环
        if (bytesRead == fileSize - sizeof(long long) - sizeof(unsigned char) && bitPos < padding_bits) {
            break;
        }
        
        int bit = (readByte >> bitPos) & 1;
        if (bit == 0) {
            if (current->left == NULL){
                printf("解码错误：遇到无效的0位，位置：字节%d, 位%d\n", bytesRead, 7-bitPos);
                // 重置到根节点，尝试重新解码
                current = huffmanTree;
            }else{
                current = current->left;
            }
        }else{
            if (current->right == NULL){
                printf("解码错误：遇到无效的1位，位置：字节%d, 位%d\n", bytesRead, 7-bitPos);
                current = huffmanTree;
            }else{
                current = current->right;
            }
        }
        
        if (current->left == NULL && current->right == NULL) {
            char utf8_buf[5] = {0};
            to_utf8(current->symbol, utf8_buf);
            fputs(utf8_buf, decode_fp);
            // printf("符号:%c 原始编码: %d\n", current->symbol < 128 ? current->symbol : ' ', current->symbol);
            charsWritten++;
            
            current = huffmanTree;
        }

        bitPos--;
        
        if (bitPos < 0) {
            if (fread(&readByte, 1, 1, source_fp) != 1) {
                break;
            }
            bytesRead++;
            bitPos = 7; 
        }
    }

    printf("解码完成：读取 %d 字节，写入 %d 个字符\n", bytesRead, charsWritten);
    freeHuffmanTree(huffmanTree);
    fclose(source_fp);
    fclose(decode_fp);
    return 0;
}

int compareFiles(const char* file1Path, const char* file2Path) {
    FILE *file1 = fopen(file1Path, "rb");
    if (file1 == NULL) {
        printf("无法打开文件：%s\n", file1Path);
        return -1;
    }

    FILE *file2 = fopen(file2Path, "rb");
    if (file2 == NULL) {
        printf("无法打开文件：%s\n", file2Path);
        fclose(file1);
        return -1;
    }

    printf("\n=== 文件比对结果 ===\n");
    printf("原始文件：%s\n", file1Path);
    printf("解码文件：%s\n", file2Path);

    // 获取文件大小
    fseek(file1, 0, SEEK_END);
    long file1Size = ftell(file1);
    fseek(file1, 0, SEEK_SET);

    fseek(file2, 0, SEEK_END);
    long file2Size = ftell(file2);
    fseek(file2, 0, SEEK_SET);

    printf("原始文件大小: %ld 字节\n", file1Size);
    printf("解码文件大小: %ld 字节\n", file2Size);

    if (file1Size != file2Size) {
        printf("警告：文件大小不匹配！\n");
    }

    // 逐字节比较文件内容
    int ch1, ch2;
    long position = 0;
    long differences = 0;
    long totalBytes = (file1Size < file2Size) ? file1Size : file2Size;

    while (((ch1 = fgetc(file1)) != EOF) && ((ch2 = fgetc(file2)) != EOF)) {
        if (ch1 != ch2) {
            differences++;
            if (differences <= 10) {
                printf("差异 #%ld: 位置 %ld - 原始文件：0x%02X, 解码文件：0x%02X\n", 
                       differences, position, ch1, ch2);
            }
        }
        position++;
    }

    // 检查文件是否完全匹配
    if (differences == 0 && file1Size == file2Size) {
        printf("文件完全匹配！解码成功!\n");
        fclose(file1);
        fclose(file2);
        return 0;
    } else {
        float matchRate = 100.0f * (totalBytes - differences) / totalBytes;
        printf("总计 %ld 个字节中有 %ld 处差异，匹配率: %.2f%%\n", 
               totalBytes, differences, matchRate);
        
        if (file1Size != file2Size) {
            printf("文件长度差异: %ld 字节\n", 
                   (file1Size > file2Size) ? (file1Size - file2Size) : (file2Size - file1Size));
        }
        
        fclose(file1);
        fclose(file2);
        return 1;
    }
}