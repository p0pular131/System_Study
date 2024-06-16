#include <stdio.h>

#define VERY_LARGE_NUMBER 1000000
#define MAX_VERTICES 8

typedef struct __edge{
    int x;
    int y;
}edge;

typedef struct __treeData{
    edge T[MAX_VERTICES * MAX_VERTICES];
    int top;
}treeData;

void init_treeData(treeData *tree)
{
    edge tmp = {-1, -1};
    for(int i = 0; i < MAX_VERTICES * MAX_VERTICES;i++)
        tree->T[i] = tmp;
    tree->top = 0;
}

void fct(int graph[MAX_VERTICES][MAX_VERTICES]);

void push_to_T(treeData *tree, int x, int y)
{
    edge tmp;
    for(int i = 0; i < tree->top; i++){
        tmp = tree->T[i];
        // 이미 존재하는 edge면 return
        if((tmp.x == x && tmp.y == y) || (tmp.x == y && tmp.y == x)){
            return;
        }
    }
    tree->T[tree->top].x = x;
    tree->T[tree->top].y = y;
    tree->top++;
}

void fct(int graph[MAX_VERTICES][MAX_VERTICES])
{
    int no_edge = 0;
    treeData tree;
    int TV[MAX_VERTICES] = {0};
    init_treeData(&tree);
    TV[0] = 1;
    while(no_edge < MAX_VERTICES - 1) {
        int min = VERY_LARGE_NUMBER;
        edge tmp = {0, 0};
        // vertex i, j의 edge가 최소값이면 추가함. 
        for (int i = 0; i < MAX_VERTICES; i++) {
            if (TV[i]) {
                for (int j = 0; j < MAX_VERTICES; j++) {
                    if (!TV[j] && graph[i][j] != -1) {
                        if (min > graph[i][j]) {
                            min = graph[i][j];
                            push_to_T(&tree, i, j);
                            tmp.x = i;
                            tmp.y = j;
                        }
                    }
                }
            }
        }
        // 찾은 edge의 tail vertex를 TV에 방문 표시
        TV[tmp.y] = true;
        no_edge++;
    }
}

int main(void)
{
    /* -1 implies there is no connection between the
    vertices. */
    int graph[MAX_VERTICES][MAX_VERTICES] = {
    { 0, 4, 8, -1, -1, -1, -1, 7},
    { 4, 0, -1, 2, 4, -1, -1, 3},
    { 8, -1, 0, -1, 15, -1, 12, -1},
    {-1, 2, -1, 0, 10, 8, -1, -1},
    {-1, 4, 15, 10, 0, -1, -1, -1},
    {-1, -1, -1, 8, -1, 0, 1, -1},
    {-1, -1, 12, -1, -1, 1, 0, 2},
    { 7, 3, -1, -1, -1, -1, 2, 0}};
    fct(graph);
    return 0;
}