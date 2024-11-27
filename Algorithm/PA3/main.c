#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

int cntASC2[128]; // 각 char가 나온 횟수를 count하는 table

typedef struct Node {
    char character;
    int frequency;
    struct Node *left, *right;
} Node;

void input_handler(FILE* input_file) {
    char ch;
    // 파일에서 문자 하나씩 읽기
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch >= 0 && ch < 128) { 
            cntASC2[(int)ch]++;
        }
    }
}

int main() {
    FILE* input_file = fopen("hw3_input.txt","r");
    FILE* output1 = fopen("hw3_output1.txt","w");
    FILE* output2 = fopen("hw3_output1.txt","w");
    if (input_file == NULL) {
        perror("Failed to open file");
        return 1;
    }   
    memset(cntASC2,0,sizeof(cntASC2)); // 모두 0회로 초기화
    input_handler(input_file);

    huffman_encoding();
    save_tree(output1);
    save_bin(output1);

    huffman_decoding();
    save_output(output2);

    fclose(input_file);
    fclose(output1);
    fclose(output2);
    return 0;
}