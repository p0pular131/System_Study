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

int N, M, R; // grid size, operations, Robot
int coinTotal = 0, coinAvg = 0;
int grid[101][101]; 

int dx[4] = {0,1,-1,0};
int dy[4] = {1,0,0,-1};

Node maxNode;
int max = -1;

void printDummy() {
    printf("1 1 1 1 0\n");
}

void printSol(Node* sol) {
    printf("%d %d %d %d %d\n",
        sol->x1, sol->y1, sol->x2, sol->y2, sol->coin);
}

int collectCoin(Node cell) {
    int x1 = cell.x1, x2 = cell.x2, y1 = cell.y1, y2 = cell.y2, coin = cell.coin;
    // 해당 영역에서의 코인들을 수집하여 grid를 update하는 함수
    for(int i=x1;i<=x2;i++) {
        for(int j=y1;j<=y2;j++) {
            grid[i][j] -= coin;
            if(grid[i][j] < 0) {
                return -1;
            }
        }
    }
    coinTotal += (x2-x1+1) * (y2-y1+1) * coin;

    return 0;
}

int calculateCost(Node cell) {
    return -1 * (cell.x2 - cell.x1 + 1) * (cell.y2 - cell.y1 +1) * cell.coin;
}

void creatSol(Node* cell) {
    int random;
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
    cell->x1 = x1; cell->x2 = x2; cell->y1 = y1; cell->y2 = y2; cell->coin = coin;
}

Node anealingSolution(double coolingRate) {
    max = -1;
    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {
            if(grid[i][j] >= max) {
                max = grid[i][j];
                maxNode.x1 = i;
                maxNode.y1 = j;
                if(max == 500) break;
            }
        }
    }
    Node currSol;
    currSol.x1 = maxNode.x1; currSol.y1 = maxNode.y1; 
    currSol.x2 = maxNode.x1; currSol.y2 = maxNode.y1; currSol.coin = max;

    Node bestSol = currSol;
    Node newSol = currSol;

    double T = 1000000.0;

    coolingRate -= (N / 10) * 0.00047; // input 사이즈에 따라 가변적으로 수렴속도 조절
    
    while(T > 0.01) {
        creatSol(&newSol);
        int currCost = calculateCost(currSol);
        int newCost = calculateCost(newSol);
        
        if((newCost < currCost) || (exp((currCost - newCost) / T) > (double)rand() / RAND_MAX)) {
            currSol = newSol;
            if(newCost < calculateCost(bestSol)) {
                bestSol = newSol;
            }
        }
        T *= coolingRate;
    }
    return bestSol;
}

void dispatchRobot() {
    int allocated = 0;
    int radius = 2; 

    while (allocated < R) {
        int minDifference = 1, bestX = 1, bestY = 1;

        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                if (grid[i][j] == 500) continue; 

                // 현재 셀의 주변 구역 평균값 계산
                int sum = 0, count = 0;
                for (int di = -radius; di <= radius; di++) {
                    for (int dj = -radius; dj <= radius; dj++) {
                        int nx = i + di, ny = j + dj;
                        if (nx >= 1 && nx <= N && ny >= 1 && ny <= N) {
                            sum += grid[nx][ny];
                            count++;
                        }
                    }
                }
                double neighborAvg = (double)sum / count;

                // 주변 평균값과 현재 셀 값의 차이 계산. 이 값이 크면 클 수록 현재 위치값이 작음을 의미
                int difference = (int)(neighborAvg - grid[i][j]);

                // 차이가 가장 큰 위치를 선택
                if (difference > minDifference) {
                    minDifference = difference;
                    bestX = i;
                    bestY = j;
                }
            }
        }

        printf("%d %d\n", bestX, bestY);
        grid[bestX][bestY] = 500; 
        allocated++;
    }
}

int main() {    
    memset(grid,-1, sizeof(grid));
    
    scanf("%d %d %d",&N,&R,&M);
    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {
            scanf("%d",&grid[i][j]);
            coinAvg += grid[i][j];
        }
    }
    coinAvg /= N*N;
    srand(time(NULL));
    clock_t startTime, endTime;
    startTime = clock();

    dispatchRobot();

    for(int i=0;i<M;i++) {
        Node best = anealingSolution(0.9999);
        if(best.coin > 0) {
            printSol(&best);
            if(collectCoin(best) == -1) {
                printf("Cannot collect coin\n");
                exit(1);
            }
        } else if(best.coin == 0) {
            printDummy();
        }
    }
    
    // 제출 전에 주석처리
    // printf("Total collect coin is %d\n",coinTotal);
    // endTime = clock();
    // printf("Elapsed Time: %f\n",(double)(endTime - startTime) / CLOCKS_PER_SEC);
    return 0;
}
