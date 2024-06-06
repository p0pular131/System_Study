#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HASHTAGS 100
#define MAX_COUNTRIES 100
#define MAX_STR_LEN 100

// 해시태그와 국가명을 저장할 배열 추가
char hashtagList[MAX_HASHTAGS][MAX_STR_LEN];
char countryList[MAX_COUNTRIES][MAX_STR_LEN];

typedef struct {
    char hashtag[100];
    char country[50];
    char date[15];
} HashtagData;

typedef struct DateNode {
    char date[15];
    int frequency;
    struct DateNode *next;
} DateNode;

typedef struct {
    DateNode *head;
} ListElement;

typedef struct {
    ListElement elements[MAX_HASHTAGS][MAX_COUNTRIES];
} HashtagCountryMatrix;

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

void parseLine(char *line, HashtagData *data) {
    sscanf(line, "%[^,], %[^,], %[^,]", data->hashtag, data->country, data->date);
}

unsigned int hashString(const char *str, int maxSize) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash % maxSize;
}

// 연결 리스트에 날짜 추가
void addDate(ListElement *element, const char *date) {
    DateNode *newNode = (DateNode *)malloc(sizeof(DateNode));
    strcpy(newNode->date, date);
    newNode->frequency = 1;
    newNode->next = NULL;

    // 리스트의 끝을 찾고 새 노드 추가
    if (element->head == NULL) {
        element->head = newNode;
    } else {
        DateNode *current = element->head;
        while (current->next != NULL) {
            if (strcmp(current->date, date) == 0) {
                current->frequency++;
                free(newNode);
                return;
            }
            current = current->next;
        }
        current->next = newNode;
    }
}

// 날짜별 빈도수 출력
void printFrequency(HashtagCountryMatrix *matrix, int hashtagIndex, int countryIndex) {
    DateNode *current = matrix->elements[hashtagIndex][countryIndex].head;
    while (current != NULL) {
        printf("Date: %s, Frequency: %d\n", current->date, current->frequency);
        current = current->next;
    }
}

// 메모리 정리
void freeMatrix(HashtagCountryMatrix *matrix) {
    for (int i = 0; i < MAX_HASHTAGS; i++) {
        for (int j = 0; j < MAX_COUNTRIES; j++) {
            DateNode *current = matrix->elements[i][j].head;
            while (current != NULL) {
                DateNode *temp = current;
                current = current->next;
                free(temp);
            }
        }
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

    HashtagCountryMatrix matrix;
    memset(&matrix, 0, sizeof(matrix));

    char *line = strtok(content, "\n");
    while (line != NULL) {
        HashtagData data;
        parseLine(line, &data);

        int hashtagIndex = hashString(data.hashtag, MAX_HASHTAGS);
        int countryIndex = hashString(data.country, MAX_COUNTRIES);

        // 해시태그와 국가명 저장
        strncpy(hashtagList[hashtagIndex], data.hashtag, MAX_STR_LEN);
        strncpy(countryList[countryIndex], data.country, MAX_STR_LEN);

        addDate(&matrix.elements[hashtagIndex][countryIndex], data.date);

        line = strtok(NULL, "\n");
    }

    // 테스트를 위해 특정 인덱스 대신 모든 데이터 출력
     for (int i = 0; i < MAX_HASHTAGS; i++) {
        for (int j = 0; j < MAX_COUNTRIES; j++) {
            if (matrix.elements[i][j].head != NULL) {
                printf("Hashtag: %s, Country: %s\n", hashtagList[i], countryList[j]);
                printFrequency(&matrix, i, j);
            }
        }
    }
    // // 필요한 경우 출력 예시
    // printFrequency(&matrix, 0, 0);  // 특정 해시태그와 국가 인덱스에 대해 출력

    free(content);
    freeMatrix(&matrix);

    return 0;
}