#include <stdio.h>
#define MAX_SIZE 10

void merge(int initList[], int mergedList[], int i, int m, int n)
{
    /*
    병합하려는 두 리스트 중, 첫 리스트의 시작점(i)&끝점(m), 두번째 리스트의 맨 끝점(n)을 input으로 받음
    첫 리스트의 시작점 : i
    첫 리스트의 끝점 : m
    두번째 리스트의 시작점 : m+1
    두번째 리스트의 끝점 : n
    각 리스트를 순회하는 index -> i, j
    최종 결과를 저장할 mergedList의 index -> k
    i번째, j번째 값을 비교하면서 작은 값을 mergedList[k]에 저장 
    */
    int j,k,t;
    j = m + 1;
    k = i;
    while(i <= m && j <= n) {
        if (initList[i] <= initList[j]){
            mergedList[k++] = initList[i++];
    }
        else{
            mergedList[k++] = initList[j++];
    }
}
    // 첫번째 리스트의 요소들이 먼저 다 들어가면 두번째 리스트의 남은 요소들 다 넣음
    if (i > m){
        for(t = j; t <= n; t++){
            mergedList[t] = initList[t];
        }
    }
    // 두번째 리스트의 요소들이 먼저 다 들어간 경우
    else{
        for(t = i; t <= m; t++){
            mergedList[k+t-i] = initList[t];
        }
    }
}

void mergePass(int initList[], int resultList[], int n, int s)
{
    int i ;
    int j ;
    // 한 줄에 대해서 각 쌍의 리스트들 병합하는 반복문
    for (i = 0; i <= n - 2 * s + 1; i += 2 * s){
        // 각 리스트 크기의 단위가 s니까 첫 리스트는 i ~ i+s-1, 두번째 리스트 끝점은 i + 2s -1
        merge(initList, resultList, i, i + s - 1, i + 2 * s - 1);
    }
    if((i + s - 1) < n){
        // 한 쌍 + alpha로 남은 경우 -> 병합 진행
        merge(initList, resultList, i, i + s - 1, n);
    }
    else{
        // 1쌍은 못이루고 병합은 되어있는 것들 -> resultList에 복사 
        for(j=i; j <= n; j++){
            resultList[j] = initList[j];
        }
    }
}

void mergeSort(int a[], int n)
{
    int s = 1;
    int extra[MAX_SIZE];
    while (s<n) {
        mergePass(a, extra, n, s);
        s *= 2;
        mergePass(extra, a, n, s);
        s *= 2;
    }
}

int main(void)
{
    int arr[MAX_SIZE] = {13, 26, 41, 72, 23, 1, 0, 65, 32, 55};
    mergeSort(arr, MAX_SIZE - 1);
}