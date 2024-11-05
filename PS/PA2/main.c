#include <stdio.h>
#include <string.h>
#include <math.h>
#define DUMMY [1,1,1,1,0]

int N, M; // grid size, operations
int grid[101][101]; 
int maskSize = 1;

void printAll() {
    int printCnt = 0;
    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {
            printf("%d %d %d %d %d\n",i,j,i,j,grid[i][j]);
            printCnt++;
        }
    }
    while(printCnt <= M) {
        printf("1 1 1 1 0\n");
        printCnt++;
    }
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
    }
    // grid에 input값 삽입
    fscanf(fp,"%d %d",&N,&M);
    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {
            fscanf(fp,"%d ",&grid[i][j]);
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