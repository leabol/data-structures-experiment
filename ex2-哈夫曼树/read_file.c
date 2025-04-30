#include <stdio.h>
#include <string.h>

#define MAX_UTF8_NUM 70000


int read_text(const char *text_path, int *char_num)
{
    FILE *fp;
    if ((fp = fopen(text_path, "rb")) == NULL){
        printf("failed open\n");
        return 1;
    }
    //读取字节，根据编码格式，转换编码存储
    int ch;
    while ((ch = fgetc(fp)) != EOF){
        int alter_utf8 = 0;
        int len = 0;

        if ((ch & 0x80) == 0x0){//   ASCII: 0XXXXXXX , 0x80 = 1000 0000d
            alter_utf8 = ch;
            char_num[alter_utf8]++;
            continue;
        } else if ((ch & 0xE0) == 0xC0){//  110XXXXX 10XXXXXX , 0xE0 = 1110 0000d
            alter_utf8 =  (ch & 0x1F);
            len = 2;
        } else if ((ch & 0xF0) == 0xE0){//  1110XXXX 10XXXXXX 10XXXXXX , 0xF0 = 1111 0000d
            alter_utf8 =  (ch &= 0xF);
            len = 3;
        }else if ((ch & 0xF8) == 0xF0){//   11110XXX 10XXXXXX 10XXXXXX 10XXXXXX , 0xF8 = 1111 1000d
            alter_utf8 = (ch & 0x7);
            len = 4;
        }else {
            printf("text's form is not utf-8\n");
            return -1;
        }

        for (int i = 1; i < len; i++){
            alter_utf8 <<= 6;
            ch = fgetc(fp);
            if ((ch & 0xC0) != 0x80){
                printf("text is not utf-8\n");
                return -1;
            }
            alter_utf8 |= (ch & 0x3F);
        }
        char_num[alter_utf8]++;
    }
    fclose(fp);
}

void transfer_to_utf8(int *char_num)
{
    int num = 0;
    int chars = 0;
    for (int i = 0; i < MAX_UTF8_NUM; i++){//i 为转换后的utf-8编码
        char utf8[5] = {0};
        if (char_num[i] != 0){
            if (i <= 0x7F){// 7bit
                utf8[0] = i;
            }else if (i <= 0x7FF){// 5 + 6 == 11bit
                utf8[0] = 0xC0 | (i >> 6);
                utf8[1] = 0x80 | (i & 0x3F);
            }else if (i <= 0xFFFF){// 4 + 6 + 6 == 16bit
                utf8[0] = 0xE0 | (i >> 12);
                utf8[1] = 0x80 | ((i >> 6) & 0x3F);
                utf8[2] = 0x80 | (i & 0x3F);
            }else{// 3 + 6 + 6 + 6 == 21bit
                utf8[0] = 0xF0 | (i >> 18);
                utf8[1] = 0x80 | ((i >> 12) & 0x3F);
                utf8[2] = 0x80 | ((i >> 6) & 0x3F);
                utf8[3] = 0x80 | (i & 0x3F);
            }
            printf("%s: %d\n", utf8, char_num[i]);
            num += char_num[i];
            chars++;
        }
    }
    printf("总结:总共%d个字符，%d个不同字符 \n", num, chars);
}
int main()
{
int char_num[MAX_UTF8_NUM];
  
    read_text("text", char_num);

    transfer_to_utf8(char_num);

}