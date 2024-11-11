#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100
#define M 100

int main() {
    FILE *fp = fopen("input_1.txt", "w");
    if (fp == NULL) {
        printf("Failed to open file.\n");
        return -1;
    }

    // 난수 생성 초기화
    srand(time(NULL));

    // 첫 번째 줄: N과 M 출력
    fprintf(fp, "%d %d\n", N, M);

    // 그리드 데이터 출력 (100x100)
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            int coinCount = rand() % 501; // 0부터 500 사이의 코인 개수
            fprintf(fp, "%d ", coinCount);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("Test case generated successfully.\n");

    return 0;
}
