#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

int cntASC2[128]; // 각 char가 나온 횟수를 count하는 table
int total_cnt = 0; // 총 node의 개수

typedef struct Node {
    char character;
    int frequency;
    struct Node *left, *right; // Huffman Tree 구현시 사용
} Node;

void input_handler(FILE* input_file) {
    char ch;
    // 파일에서 문자 하나씩 읽기
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch >= 0 && ch < 128) cntASC2[(int)ch]++;
    }
}

void heapify_up(int minmax, Node** pq) {
    int i = total_cnt - 1; 
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (minmax == 0) { // min heap
            if (pq[parent]->frequency > pq[i]->frequency) {
                Node* temp = pq[parent];
                pq[parent] = pq[i];
                pq[i] = temp;
            }
            else break;
        }
        else { // max heap
            if (pq[parent]->frequency < pq[i]->frequency) {
                Node* temp = pq[parent];
                pq[parent] = pq[i];
                pq[i] = temp;
            }
            else break;
        }
        i = parent;
    }
}

void heapify_down(int minmax, Node** pq) {
    int i = 0;
    while (i * 2 + 1 < total_cnt) {
        int left = i * 2 + 1;
        int right = i * 2 + 2;
        int child;
        if (minmax == 0) { // min heap
            if (right >= total_cnt) child = left;
            else child = (pq[left]->frequency < pq[right]->frequency) ? left : right;
            if (pq[i]->frequency > pq[child]->frequency) {
                Node* temp = pq[i];
                pq[i] = pq[child];
                pq[child] = temp;
            }
            else break;
        }
        else { // max heap
            if (right >= total_cnt) child = left;
            else child = (pq[left]->frequency > pq[right]->frequency) ? left : right;
            if (pq[i]->frequency < pq[child]->frequency) {
                Node* temp = pq[i];
                pq[i] = pq[child];
                pq[child] = temp;
            }
            else break;
        }
        i = child;
    }
}

void create_pq(Node** pq) {
    for (int i = 0; i < 128; i++) {
        // insert node
        if (cntASC2[i] != 0) {
            Node* new_node = (Node*)malloc(sizeof(Node));
            new_node->character = (char)i;
            new_node->frequency = cntASC2[i];
            new_node->left = NULL;
            new_node->right = NULL;
            pq[total_cnt] = new_node;
            total_cnt++;
            heapify_up(0,pq); // minheap -> 0, maxheap -> 1
        }
    }
}

Node* dequeue(Node** pq) {
    Node* temp = pq[0];
    pq[0] = pq[total_cnt - 1];
    pq[total_cnt - 1] = temp;
    total_cnt--;
    heapify_down(0,pq);
    return temp;
}

void enqueue(Node** pq, Node* new_node) {
    pq[total_cnt++] = new_node;
    heapify_up(0,pq);
}

Node* huffman_encoding(Node** pq) {
    while (total_cnt > 1) {
        Node* left = dequeue(pq);
        Node* right = dequeue(pq);

        Node* new_node = (Node*)malloc(sizeof(Node));
        if (new_node == NULL) {
            printf("Error: Memory allocation failed\n");
            exit(1);
        }

        new_node->left = left;
        new_node->right = right;
        new_node->frequency = left->frequency + right->frequency;
        new_node->character = 0;

        enqueue(pq, new_node);
    }
    return pq[0];
}

void save_tree(FILE* output, Node* root) {
    if (root == NULL) {
        printf("Error: root is NULL\n");
        return;
    }
    if (root->left == NULL && root->right == NULL) {
        // 리프 노드일 경우 해당 문자를 출력
        fprintf(output, "%c", root->character);
    } else {
        // 내부 노드일 경우 괄호로 묶어서 출력
        fprintf(output, "(");
        save_tree(output, root->left);
        fprintf(output, ",");
        save_tree(output, root->right);
        fprintf(output, ")");
    }
}

// Huffman Tree에서 각 문자에 대한 Huffman 코드를 생성
void generate_huffman_code(Node* root, char* code, int depth, char** code_table) {
    if (root == NULL) {
        printf("Error: root is NULL\n");
        return;
    }
    if (root->left == NULL && root->right == NULL) {
        // 리프 노드일 경우 해당 코드 저장
        code[depth] = '\0';  // 현재 위치에서 문자열 끝 지정
        code_table[(int)root->character] = strdup(code);
    } else {
        // 왼쪽 자식 노드로 이동 (0 추가)
        code[depth] = '0';
        generate_huffman_code(root->left, code, depth + 1, code_table);

        // 오른쪽 자식 노드로 이동 (1 추가)
        code[depth] = '1';
        generate_huffman_code(root->right, code, depth + 1, code_table);
    }
}

// Huffman Tree 기반으로 파일을 압축하여 저장
void save_bin(FILE* output, Node* root, FILE* input_file) {
    if (root == NULL) {
        printf("Error: root is NULL\n");
        return;
    }
    // Huffman 코드 테이블 생성
    char* code_table[128] = {0};
    char code[128];
    generate_huffman_code(root, code, 0, code_table);

    // 입력 파일 다시 읽기
    fseek(input_file, 0, SEEK_SET);
    char ch;
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch >= 0 && ch < 128) {
            // 각 문자의 Huffman 코드를 출력 파일에 저장
            fprintf(output, "%s", code_table[(int)ch]);
        }
    }

    // 메모리 해제
    for (int i = 0; i < 128; i++) {
        if (code_table[i] != NULL) free(code_table[i]);
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

    Node** pq = (Node**)malloc(sizeof(Node*)*128); 
    create_pq(pq); // 최초 input data MinHeap 생성

    // MinHeap debuging
    // printf("Min Heap : \n");
    // for(int i=0;i<total_cnt;i++) { 
    //     printf("%c %d\n",pq[i]->character,pq[i]->frequency);
    // }

    Node* root = huffman_encoding(pq); // Huffman Tree 생성
    save_tree(output1,root); // tree를 (())형식으로 저장 
    fprintf(output1, "\n");
    save_bin(output1,root,input_file);  // encoding 결과 압축된 binary 저장.
    fclose(output1);

    // huffman_decoding();
    // save_output(output2);

    fclose(input_file);
    fclose(output2);
    return 0;
}