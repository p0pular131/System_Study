#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char hashtag[100];
    char country[50];
    char date[15];
    int frequency;
} HashtagInfo;

typedef struct Node {
    HashtagInfo data;
    struct Node *next;
} Node;

#define MAX_HASHTAGS 1000

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

void parseLine(char *line, HashtagInfo *info) {
    sscanf(line, "%[^,], %[^,], %[^,]", info->hashtag, info->country, info->date);
    info->frequency = 1; // 각 줄은 최소 한 번의 해시태그 사용을 의미
}

Node* createNode(HashtagInfo *info) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = *info;
    newNode->next = NULL;
    return newNode;
}

Node* addOrUpdateHashtag(Node *head, HashtagInfo *newInfo) {
    Node *current = head;
    Node *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->data.hashtag, newInfo->hashtag) == 0 &&
            strcmp(current->data.country, newInfo->country) == 0 &&
            strcmp(current->data.date, newInfo->date) == 0) {
            current->data.frequency++;
            return head;
        }
        previous = current;
        current = current->next;
    }

    Node *newNode = createNode(newInfo);
    if (previous == NULL) {
        head = newNode;
    } else {
        previous->next = newNode;
    }
    return head;
}

int compareByHashtag(const void *a, const void *b) {
    HashtagInfo *infoA = (HashtagInfo *)a;
    HashtagInfo *infoB = (HashtagInfo *)b;
    int result = strcmp(infoA->hashtag, infoB->hashtag);
    if (result == 0) {
        result = strcmp(infoA->country, infoB->country);
        if (result == 0) {
            result = strcmp(infoA->date, infoB->date);
        }
    }
    return result;
}

int compareByCountry(const void *a, const void *b) {
    HashtagInfo *infoA = (HashtagInfo *)a;
    HashtagInfo *infoB = (HashtagInfo *)b;
    int result = strcmp(infoA->country, infoB->country);
    if (result == 0) {
        result = strcmp(infoA->hashtag, infoB->hashtag);
        if (result == 0) {
            result = strcmp(infoA->date, infoB->date);
        }
    }
    return result;
}
void printLinkedList(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("Hashtag: %s, Country: %s, Date: %s, Frequency: %d\n",
               current->data.hashtag, current->data.country, current->data.date, current->data.frequency);
        current = current->next;
    }
}

void bubbleSortLinkedList(Node **head, int (*compare)(const void *, const void *)) {
    int swapped;
    Node *ptr1;
    Node *lptr = NULL;

    if (*head == NULL) return;

    do {
        swapped = 0;
        ptr1 = *head;

        while (ptr1->next != lptr) {
            if (compare(&(ptr1->data), &(ptr1->next->data)) > 0) {
                HashtagInfo temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = 1;
            }

            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void freeLinkedList(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
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
    Node *head = NULL;
    char *line = strtok(content, "\n");

    while (line != NULL) {
        HashtagInfo newInfo;
        parseLine(line, &newInfo);
        head = addOrUpdateHashtag(head, &newInfo);
        line = strtok(NULL, "\n");
    }

    // 해시태그 기준으로 버블 정렬
    bubbleSortLinkedList(&head, compareByHashtag);
    printf("Data Sorted by Hashtag:\n");
    printLinkedList(head);
    // 국가 기준으로 버블 정렬
    bubbleSortLinkedList(&head, compareByCountry);
    printf("Data Sorted by Hashtag:\n");
    printLinkedList(head);
    
    // 메모리 해제 및 종료
    free(content);
    // 연결 리스트 메모리 해제 함수 (구현 필요)
    freeLinkedList(head);

    return 0;
}