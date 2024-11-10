#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

struct Node {
    int x1;
    int y1;
    int x2;
    int y2;
    int coin; // total 아니고 각 cell마다 수집할 코인 개수
}typedef Node;

int N, M; // grid size, operations
int grid[101][101]; 
int gridAvg[101][101];
int maskSize = 1;
int totalSum = 0, avg = 0;
int buff_len = 0;
Node buff[100];

void printDummy() {
    printf("1 1 1 1 0\n");
}

void printAll() {
    int printCnt = 0;
    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {
            printf("%d %d %d %d %d\n",i,j,i,j,grid[i][j]);
            printCnt++;
        }
    }
    while(printCnt <= M) {
        printDummy();
        printCnt++;
    }
}

int collectCoin(Node cell) {
    int x1 = cell.x1, x2 = cell.x2, y1 = cell.y1, y2 = cell.y2, coin = cell.coin;
    // 해당 영역에서의 코인들을 수집하여 grid를 update하는 함수
    for(int i=x1;i<=x2;i++) {
        for(int j=y1;j<=y2;j++) {
            grid[i][j] -= coin;
            // invalid operation -> termiante abnormally
            if(grid[i][j] < 0) {
                return -1;
            }
        }
    }
    return 0;
}

int main() {
    FILE *fp;
    fp = fopen("input.txt","r");
    if(fp == NULL) {
       printf("Failed to open\n");
       return -1;
    }
    // -1로 grid 초기화
    for(int i=0;i<101;i++) {
        memset(grid[i],-1,101);
        memset(gridAvg[i],-1,101);
    }
    // grid에 input값 삽입
    fscanf(fp,"%d %d",&N,&M);
    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {
            fscanf(fp,"%d ",&grid[i][j]);
            totalSum += grid[i][j];
        }
    }
    avg = totalSum / (N*N);

    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {
            if(grid[i][j] >= avg) gridAvg[i][j] = grid[i][j];
            else gridAvg[i][j] = -1;
        }
    }

    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {

        }
    }

    // 1. N*N <= M : grid 하나씩 잡아서 모든 coin 다 털기 
    if(N*N <= M) {
        printAll();
    }
    // 2. N < M : 
    else if(N <= 20) {
        maskSize = 5;
    }
    // 3. 
    else if(N > 20 && N <= 50) {
        maskSize = 10;
    }
    else if(N>50 && N <= 70) {

    }
    else if(N > 70) {

    }

    return 0;
}