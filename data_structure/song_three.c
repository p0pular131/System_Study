// 3-dimensional array로 구현

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char hashtag[100];
    char country[50];
    char date[15];
    int frequency;
} HashtagData;

#define MAX_COUNTRIES 1000  // 최대 국가 수
#define MAX_HASHTAGS 1000   // 최대 해시태그 수
#define MAX_DATES 1000      // 최대 날짜 수
#define MAX_NAME_LENGTH 1000  // 이름 최대 길이

int frequencyMatrix[MAX_COUNTRIES][MAX_HASHTAGS][MAX_DATES];
char countryNames[MAX_COUNTRIES][MAX_NAME_LENGTH];
char hashtagNames[MAX_HASHTAGS][MAX_NAME_LENGTH];


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

void initializeFrequencyMatrix() {
    for (int i = 0; i < MAX_COUNTRIES; i++) {
        for (int j = 0; j < MAX_HASHTAGS; j++) {
            for (int k = 0; k < MAX_DATES; k++) {
                frequencyMatrix[i][j][k] = 0;
            }
        }
    }
}

int hashString(const char *str, int maxSize) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash % maxSize;
}

int getCountryIndex(const char* country) {
    return hashString(country, MAX_COUNTRIES);
}

int getHashtagIndex(const char* hashtag) {
    return hashString(hashtag, MAX_HASHTAGS);
}

int getDateIndex(const char* date) {
    return hashString(date, MAX_DATES);
}

void parseLine(char *line, HashtagData *data) {
    sscanf(line, "%[^,], %[^,], %[^,]", data->hashtag, data->country, data->date);
}

void processContent(char *content) {
    char *line = strtok(content, "\n");
    while (line != NULL) {
        HashtagData newInfo;
        parseLine(line, &newInfo);

        int countryIndex = getCountryIndex(newInfo.country);
        int hashtagIndex = getHashtagIndex(newInfo.hashtag);
        int dateIndex = getDateIndex(newInfo.date);


        strcpy(countryNames[countryIndex], newInfo.country);
        strcpy(hashtagNames[hashtagIndex], newInfo.hashtag);

        frequencyMatrix[countryIndex][hashtagIndex][dateIndex]++;

        line = strtok(NULL, "\n");
    }
}

void printFrequencyMatrix() {
    for (int i = 0; i < MAX_COUNTRIES; i++) {
        for (int j = 0; j < MAX_HASHTAGS; j++) {
            for (int k = 0; k < MAX_DATES; k++) {
                if (frequencyMatrix[i][j][k] > 0) {
                    printf("Country: %s, Hashtag: %s, Date Index: %d, Frequency: %d\n",
                           countryNames[i], hashtagNames[j], k, frequencyMatrix[i][j][k]);
                }
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

    // for(int k=0;k<10000;k++){
    //     for(int q=0;q<10000;q++) {
    //         for(int w=0;w<700;w++) {}
    //     }
    // }

    initializeFrequencyMatrix();
    processContent(content);
    printFrequencyMatrix();

    free(content);
    return 0;
}