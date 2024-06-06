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

int findHashtag(HashtagInfo *hashtags, int count, HashtagInfo *newInfo) {
    for (int i = 0; i < count; i++) {
        if (strcmp(hashtags[i].hashtag, newInfo->hashtag) == 0 &&
            strcmp(hashtags[i].country, newInfo->country) == 0 &&
            strcmp(hashtags[i].date, newInfo->date) == 0) {
            return i;
        }
    }
    return -1;
}


void addOrUpdateHashtag(HashtagInfo *hashtags, int *count, HashtagInfo *newInfo) {
    int index = findHashtag(hashtags, *count, newInfo);
    if (index != -1) {
        hashtags[index].frequency++;
    } else {
        if (*count < MAX_HASHTAGS) {
            hashtags[*count] = *newInfo;
            (*count)++;
        }
    }
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

void printHashtags(const HashtagInfo *hashtags, int count) {
    for (int i = 0; i < count; i++) {
        printf("%s, %s, %s, %d\n", hashtags[i].hashtag, hashtags[i].country, hashtags[i].date, hashtags[i].frequency);
    }
}

void processContent(char *content, HashtagInfo *hashtags, int *count) {
    char *line = strtok(content, "\n");
    while (line != NULL && *count < MAX_HASHTAGS) {
        HashtagInfo newInfo;
        parseLine(line, &newInfo);
        addOrUpdateHashtag(hashtags, count, &newInfo);
        line = strtok(NULL, "\n");
    }
}

void bubbleSortByHashtag(HashtagInfo *hashtags, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (compareByHashtag(&hashtags[j], &hashtags[j + 1]) > 0) {
                HashtagInfo temp = hashtags[j];
                hashtags[j] = hashtags[j + 1];
                hashtags[j + 1] = temp;
            }
        }
    }
}

void bubbleSortByCountry(HashtagInfo *hashtags, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (compareByCountry(&hashtags[j], &hashtags[j + 1]) > 0) {
                HashtagInfo temp = hashtags[j];
                hashtags[j] = hashtags[j + 1];
                hashtags[j + 1] = temp;
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
    for(int k=0;k<10000;k++){
        for(int q=0;q<10000;q++) {
            for(int w=0;w<700;w++) {}
        }
    }
    HashtagInfo hashtags[MAX_HASHTAGS];
    int count = 0;
    processContent(content, hashtags, &count);

    // 해시태그 기준으로 버블 정렬
    bubbleSortByHashtag(hashtags, count);
    printf("Sorted by Hashtag:\n");
    printHashtags(hashtags, count);

    // 국가 기준으로 버블 정렬
    bubbleSortByCountry(hashtags, count);
    printf("\nSorted by Country:\n");
    printHashtags(hashtags, count);

    free(content);
    return 0;
}
