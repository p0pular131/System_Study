// binary tree로 구현

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char hashtag[100];
    char country[50];
    char date[15];
    int frequency;
} HashtagInfo;

#define MAX_HASHTAGS 1000

typedef struct BSTNode {
    HashtagInfo data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

void parseLine(char *line, HashtagInfo *info) {
    sscanf(line, "%[^,], %[^,], %[^,]", info->hashtag, info->country, info->date);
    info->frequency = 1; // 각 줄은 최소 한 번의 해시태그 사용을 의미
}


// =============== 1. fopen을 이용한 file open ===============
FILE *txt_open(const char *fileName) {
    FILE *file = fopen(fileName, "r");

    return file;
}

// =============== 2. File의 Size 계산 ===============
long txt_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    return fileSize;
}

// =============== 3. File의 내용 얻기 ===============
char *txt_content(FILE *file, long fileSize) {
    char *content = (char *)malloc(sizeof(char) * (fileSize + 1));

    size_t bytesRead = fread(content, 1, fileSize, file);   

    content[fileSize] = '\0';
    return content;
}

// =============== 4. file 읽는 메인 함수 ===============
char *txt_read(const char *fileName) {
    FILE *file = txt_open(fileName);
    if (file == NULL) return NULL;

    long txtsize = txt_size(file);
    char *content = txt_content(file, txtsize);
    fclose(file);

    return content;
}

BSTNode* insertBSTNode(BSTNode *root, HashtagInfo *newInfo) {
    if (root == NULL) {
        BSTNode *newNode = (BSTNode*)malloc(sizeof(BSTNode));
        newNode->data = *newInfo;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    int compareResult = strcmp(newInfo->hashtag, root->data.hashtag);
    if (compareResult == 0) {
        compareResult = strcmp(newInfo->country, root->data.country);
        if (compareResult == 0) {
            compareResult = strcmp(newInfo->date, root->data.date);
            if (compareResult == 0) {
                root->data.frequency += newInfo->frequency; // 같은 날짜에 대한 빈도수 누적
                return root;
            }
        }
    }

    if (compareResult < 0) {
        root->left = insertBSTNode(root->left, newInfo);
    } else {
        root->right = insertBSTNode(root->right, newInfo);
    }

    return root;
}

void inorderTraversalByHashtag(BSTNode *root) {
    if (root != NULL) {
        inorderTraversalByHashtag(root->left);

        printf("%s, %s, %s, %d\n", root->data.hashtag, root->data.country, root->data.date, root->data.frequency);
        inorderTraversalByHashtag(root->right);
    }
}

void inorderTraversalByCountry(BSTNode *root) {
    if (root != NULL) {
        inorderTraversalByCountry(root->left);

        printf("%s, %s, %s, %d\n", root->data.country, root->data.hashtag, root->data.date, root->data.frequency);
        inorderTraversalByCountry(root->right);
    }
}

void freeBST(BSTNode *root) {
    if (root != NULL) {
        freeBST(root->left);
        freeBST(root->right);
        free(root);
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *fileName = argv[1];
    char *content = txt_read(fileName);
    if (content == NULL) {
        printf("Failed to read file.\n");
        return 1;
    }
    // for(int k=0;k<10000;k++){
    //     for(int q=0;q<10000;q++) {
    //         for(int w=0;w<700;w++) {}
    //     }
    // }
    BSTNode *root = NULL;
    char *line = strtok(content, "\n");
    while (line != NULL) {
        HashtagInfo newInfo;
        parseLine(line, &newInfo);
        root = insertBSTNode(root, &newInfo);
        line = strtok(NULL, "\n");
    }

    printf("Data Sorted by Hashtag:\n");
    inorderTraversalByHashtag(root);

    printf("\nData Sorted by Country:\n");
    inorderTraversalByCountry(root);

    free(content);
    freeBST(root);
    return 0;
}