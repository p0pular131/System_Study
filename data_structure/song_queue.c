#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HASHTAGS 1000
#define MAX_COUNTRIES 1000
#define MAX_DATE_LENGTH 15
#define MAX_COUNTRY_LENGTH 50
#define MAX_QUEUES 10

typedef struct DateNode {
    char date[MAX_DATE_LENGTH];
    int frequency;
    struct DateNode *next;
} DateNode;

typedef struct QueueNode {
    char country[MAX_COUNTRY_LENGTH];
    DateNode *dateHead;
    struct QueueNode *link;
} QueueNode;

typedef QueueNode *QueuePointer;

QueuePointer front[MAX_QUEUES], rear[MAX_QUEUES];

FILE *txt_open(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    return file;
}

long txt_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    return fileSize;
}

char *txt_content(FILE *file, long fileSize) {
    char *content = (char *)malloc(sizeof(char) * (fileSize + 1));
    size_t bytesRead = fread(content, 1, fileSize, file);   
    content[fileSize] = '\0';
    return content;
}

char *txt_read(const char *fileName) {
    FILE *file = txt_open(fileName);
    if (file == NULL) return NULL;
    long txtsize = txt_size(file);
    char *content = txt_content(file, txtsize);
    fclose(file);
    return content;
}

void parseLine(char *line, char *hashtag, char *country, char *date) {
    sscanf(line, "%[^,], %[^,], %[^,]", hashtag, country, date);
}

unsigned int hashString(const char *str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash % MAX_QUEUES;
}

void enqueue(QueuePointer *front, QueuePointer *rear, char *country, char *date) {
    QueueNode *temp;

    // 이미 존재하는 큐 노드를 찾음
    for (temp = *front; temp != NULL; temp = temp->link) {
        if (strcmp(temp->country, country) == 0) {
            // 날짜 노드 탐색 및 업데이트
            DateNode *currentDate = temp->dateHead;
            while (currentDate != NULL) {
                if (strcmp(currentDate->date, date) == 0) {
                    currentDate->frequency++;
                    return;
                }
                currentDate = currentDate->next;
            }

            // 새 날짜 노드 추가
            DateNode *newDate = (DateNode *)malloc(sizeof(DateNode));
            strcpy(newDate->date, date);
            newDate->frequency = 1;
            newDate->next = temp->dateHead;
            temp->dateHead = newDate;
            return;
        }
    }

    // 새 큐 노드 생성
    temp = (QueueNode *)malloc(sizeof(QueueNode));
    strcpy(temp->country, country);
    temp->dateHead = NULL;
    temp->link = NULL;

    // 새 날짜 노드 추가
    DateNode *newDate = (DateNode *)malloc(sizeof(DateNode));
    strcpy(newDate->date, date);
    newDate->frequency = 1;
    newDate->next = temp->dateHead;
    temp->dateHead = newDate;

    // 큐에 새 노드 추가
    if (*rear) {
        (*rear)->link = temp;
    } else {
        *front = temp;
    }
    *rear = temp;
}

void printQueue(QueuePointer front) {
    QueueNode *currentQueue = front;
    while (currentQueue) {
        printf("Country: %s\n", currentQueue->country);
        DateNode *currentDate = currentQueue->dateHead;
        while (currentDate) {
            printf("  Date: %s, Frequency: %d\n", currentDate->date, currentDate->frequency);
            currentDate = currentDate->next;
        }
        currentQueue = currentQueue->link;
    }
}

void printAllQueues(QueuePointer front[], int numQueues) {
    for (int i = 0; i < numQueues; i++) {
        if (front[i] != NULL) {
            printf("Queue for Hashtag Index %d:\n", i);
            printQueue(front[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *fileName = argv[1];
    FILE *file = fopen(fileName, "r");
    if (!file) {
        printf("Failed to open file.\n");
        return 1;
    }

    char line[256], hashtag[100], country[MAX_COUNTRY_LENGTH], date[MAX_DATE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        parseLine(line, hashtag, country, date);
        int queueIndex = hashString(hashtag);
        enqueue(&front[queueIndex], &rear[queueIndex], country, date);
    }

    fclose(file);

    // Print example for the first queue
    printAllQueues(front, MAX_QUEUES);

    // Free memory (not implemented)

    return 0;
}