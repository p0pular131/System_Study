#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

struct Node { // 한번의 operation 경우의 수를 저장할 구조체
    int x1;
    int y1;
    int x2;
    int y2;
    int coin; // total 아니고 각 cell마다 수집할 코인 개수
}typedef Node;

int N, M; // grid size, operations
int coinTotal = 0;
int grid[101][101]; 

int dx[4] = {0,1,-1,0};
int dy[4] = {1,0,0,-1};

void printDummy() {
    printf("1 1 1 1 0\n");
}

void printSol(Node* sol) {
    printf("%d %d %d %d %d\n",
        sol->x1, sol->y1, sol->x2, sol->y2, sol->coin);
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
   coinTotal += (x2-x1+1) * (y2-y1+1) * coin;

    return 0;
}

int calculateCost(Node cell) {
    // 해당 영역에서 수집 가능한 총 coin의 -만큼 반환
    return -1 * (cell.x2 - cell.x1 + 1) * (cell.y2 - cell.y1 +1) * cell.coin;
}

void creatSol(Node* cell) {
    int random = 0;
    int x1, x2, y1, y2;
    
    random = rand() % 4;
    x1 = cell->x1 + dx[random];
    y1 = cell->y1 + dy[random];
    
    random = rand() % 4;
    x2 = cell->x2 + dx[random];
    y2 = cell->y2 + dy[random];

    if((x1>x2) || (y1>y2)) {
        return;
    }
    if((x1<=0) || (y1<=0) || (x2>N) || (y2>N)) {
        return;
    }

    int coin = 501;
    for(int i=x1;i<=x2;i++) {
        for(int j=y1;j<=y2;j++) {
            if(coin > grid[i][j]) {
                coin = grid[i][j];
            }
        }
    }
    // update cell
    cell->x1 = x1; cell->x2 = x2; cell->y1 = y1; cell->y2 = y2; cell->coin = coin;
}

Node anealingSolution() {
    Node currSol;
    currSol.x1 = 1; currSol.y1 = 1; currSol.x2 = 1; currSol.y2 = 1; currSol.coin = grid[1][1];
    Node bestSol = currSol;
    Node newSol = currSol;

    double T = 100000.0;
    double coolingRate = 0.99;
    
    while(T > 0.01) {
        creatSol(&newSol);
        int currCost = calculateCost(currSol);
        int newCost = calculateCost(newSol);
        
        if((newCost < currCost) || (exp((currCost - newCost) / T) > (double)rand() / RAND_MAX)) {
            currSol = newSol;
            // 더 나은 솔루션이면 bestSol 갱신
            if(newCost < calculateCost(bestSol)) {
                bestSol = newSol;
            }
        }
        
        // 온도 감소
        T *= coolingRate;
    }
    return bestSol;
}

int main() {
    // input만 제출 전에 stdin으로 변경하기
    FILE *fp;
    fp = fopen("input_1.txt","r");
    if(fp == NULL) {
       printf("Failed to open\n");
       return -1;
    }
    // -1로 grid 초기화
    memset(grid,-1, sizeof(grid));
    
    // grid에 input값 삽입
    fscanf(fp,"%d %d",&N,&M);
    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {
            fscanf(fp,"%d ",&grid[i][j]);
        }
    }
    // rand에 무작위 시드 부여
    srand(time(NULL));

    for(int i=0;i<M;i++) {
        Node best = anealingSolution();
        if(best.coin > 0) {
            printSol(&best);
        }
        else if(best.coin == 0) {
            printDummy();
        }
        // update grid
        collectCoin(best);
    }
    // 제출 전에 수정
    printf("Total collect coin is %d\n",coinTotal);
    return 0;
}