#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

#define ASCLEN 128
int cntASC2[ASCLEN]; // 각 char가 나온 횟수를 count하는 table
int total_cnt = 0; // 총 node의 개수
int tree_depth;

typedef unsigned int uint;

typedef struct Node {
    char character;
    uint frequency;
    struct Node *left, *right; // Huffman Tree 구현시 사용
} Node;

void input_handler(FILE* input_file) {
    char ch;
    // 파일에서 문자 하나씩 읽기
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch >= 0 && ch < ASCLEN) {
            cntASC2[(int)ch]++;
        }
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
            // 왼쪽 자식 노드와 오른쪽 자식 노드 중 더 작은 노드 선택, 같으면 왼쪽을 우선 선택
            else child = (pq[left]->frequency <= pq[right]->frequency) ? left : right;
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
    for (int i = 0; i < ASCLEN; i++) {
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

int calculate_depth(Node* root) {
    if (root == NULL) {
        return 0; 
    }
    int left_depth = calculate_depth(root->left);
    int right_depth = calculate_depth(root->right);

    return 1 + ((left_depth > right_depth) ? left_depth : right_depth);
}

void save_tree(FILE* output, Node* root) {
    // tree를 .json형식으로 저장
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL) {
        fprintf(output, "{\"char\":\"%c\"}", root->character);
    } 
    else {
        fprintf(output, "{\"left\":");
        save_tree(output, root->left);
        fprintf(output, ",\"right\":");
        save_tree(output, root->right);
        fprintf(output, "}");
    }
}

void generate_huffman_code(Node* root, char* code, int depth, char** code_table) {
    // 각 아스키 코드에 대한 huffman code를 생성
    if (root == NULL) {
        printf("Error: root is NULL\n");
        return;
    }
    if (root->left == NULL && root->right == NULL) {
        code[depth] = '\0'; 
        code_table[(int)root->character] = strdup(code);
    } 
    else {
        // 왼쪽 -> 0
        code[depth] = '0';
        generate_huffman_code(root->left, code, depth + 1, code_table);

        // 오른쪽 -> 1
        code[depth] = '1';
        generate_huffman_code(root->right, code, depth + 1, code_table);
    }
}

void save_bin(FILE* input_file, FILE* output, Node* root) {
    if (root == NULL) {
        printf("Error: root is NULL\n");
        return;
    }
    char* huffcode_table[ASCLEN] = {0}; // 각 char에 대한 huffman code를 저장하는 table
    char huffcode[tree_depth];
    generate_huffman_code(root, huffcode, 0, huffcode_table);

    fseek(input_file, 0, SEEK_SET); // 파일 포인터를 처음으로 이동
    char ch;
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch >= 0 && ch < ASCLEN) {
            fprintf(output, "%s", huffcode_table[(int)ch]);
        }
    }

    // 메모리 해제
    for (int i = 0; i < ASCLEN; i++) {
        if (huffcode_table[i] != NULL) free(huffcode_table[i]);
    }
}

void save_output(FILE* output, Node* root, FILE* input_file) {
    if (root == NULL) {
        printf("Error: root is NULL\n");
        return;
    }
    char* huffcode_table[ASCLEN] = {0};
    char huffcode[tree_depth];
    generate_huffman_code(root, huffcode, 0, huffcode_table);

    for(int i=0;i<ASCLEN;i++) {
        if(huffcode_table[i] != NULL) {
            fprintf(output, "%c:%s\n", i, huffcode_table[i]);
        }
    }

    for (int i = 0; i < ASCLEN; i++) {
        if (huffcode_table[i] != NULL) free(huffcode_table[i]);
    }
}

void free_tree(Node* root) {
    if(root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void move_FP_secondline(FILE* input) {
    // 파일 포인터를 두 번째 줄로 이동
    fseek(input, 0, SEEK_SET); // 먼저 포인터 위치를 처음으로 이동
    char buffer[1024]; // 임시 버퍼 크기
    while (fgets(buffer, sizeof(buffer), input)) {
        if (strchr(buffer, '\n')) break;
    }
}

Node* parse_json_tree(FILE* input) {
    char ch;
    if (fscanf(input, " { \"%c", &ch) != 1) return NULL;

    if (ch == 'c') {
        // 리프 노드 {"char":"<문자>"}
        char character;
        fscanf(input, "har\":\"%c\" }", &character);

        Node* node = (Node*)malloc(sizeof(Node));
        if (node == NULL) {
            printf("Error: Memory allocation failed\n");
            exit(1);
        }
        node->character = character;
        node->left = node->right = NULL;
        return node;
    } 
    else if (ch == 'l') {
        // 내부 노드 {"left": ... , "right": ... }
        Node* node = (Node*)malloc(sizeof(Node));
        if (node == NULL) {
            printf("Error: Memory allocation failed\n");
            exit(1);
        }
        node->character = 0; // 내부 노드는 문자가 없음

        fscanf(input, "eft\":");
        node->left = parse_json_tree(input);

        fscanf(input, ",\"right\":");
        node->right = parse_json_tree(input);

        // 내부 노드 종료
        fscanf(input, " }");

        return node;
    }

    return NULL;
}

void decode_huff(FILE* input, FILE* output, Node* root) {
    if (root == NULL) {
        printf("Error: Huffman tree root is NULL\n");
        return;
    }

    char bit;
    Node* current = root;

    while ((bit = fgetc(input)) != EOF) {
        if (bit == '0') {
            current = current->left;
        } 
        else if (bit == '1') {
            current = current->right;
        } 
        else if (bit == '\n') {
            break;
        }

        if (current->left == NULL && current->right == NULL) {
            fprintf(output, "%c", current->character); 
            current = root; 
        }
    }
}

void save_output2(FILE* input_file, FILE* output, Node* root, char** code_table) {
    // huffcode 먼저 저장
    for (int i = 0; i < 128; i++) {
        if (code_table[i] != NULL) {
            fprintf(output, "%c:%s\n", (char)i, code_table[i]);
        }
    }
    move_FP_secondline(input_file);
    // huffcode decoding 결과 저장
    decode_huff(input_file, output, root);
}

int main() {
//================================================================================================
// Huffman Encoding
    FILE* input_file = fopen("hw3_input.txt","r");
    FILE* output1 = fopen("hw3_output1.txt","w");
    if (input_file == NULL) {
        perror("Failed to open file");
        return 1;
    }   
    memset(cntASC2,0,sizeof(cntASC2)); 
    input_handler(input_file);
    Node** pq = (Node**)malloc(sizeof(Node*)*ASCLEN); 
    create_pq(pq); // 최초 input data MinHeap 생성

    // MinHeap debuging
    // printf("Min Heap : \n");
    // for(int i=0;i<total_cnt;i++) { 
    //     printf("%c %d\n",pq[i]->character,pq[i]->frequency);
    // }

    Node* root = huffman_encoding(pq); // Huffman Tree 생성
    tree_depth = calculate_depth(root); // Tree의 depth 계산
    save_tree(output1,root); // tree를 (())형식으로 저장 
    fprintf(output1, "\n");
    save_bin(input_file,output1,root);  // encoding 결과 압축된 binary 저장.

    free_tree(root); // 트리 메모리 해제
    fclose(input_file);
    fclose(output1);
//================================================================================================

//================================================================================================
// Huffman Decoding
    FILE* input2 = fopen("hw3_output1.txt","r");
    FILE* output2 = fopen("hw3_output2.txt","w");
    char* asc2huff_table[ASCLEN] = {0};
    char asc2huff[tree_depth]; 

    Node* root2 = parse_json_tree(input2);
    tree_depth = calculate_depth(root2);
    generate_huffman_code(root2,asc2huff,0,asc2huff_table);
    save_output2(input2,output2,root2,asc2huff_table);

    free_tree(root2);
    for(int i=0;i<ASCLEN;i++) {
        if(asc2huff_table[i] != NULL) free(asc2huff_table[i]);
    }
    fclose(input2);
    fclose(output2);
    return 0;
}