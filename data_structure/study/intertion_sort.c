#include <stdio.h>

typedef struct {
    int key;
    int data;
}element;

void insert(element e, element a[], int i) {
    // 0번째 인덱스에 정렬 기준 요소 삽입 -> 종료조건 때문에
    a[0] = e;
    // 정렬하려는 요소를 앞의 요소들을 순서대로 탐색하며 정렬
    // 앞의 요소가 자기보다 작거나 같은걸 마주하면 거기가 요소의 정렬 위치 -> break
    while(e.key < a[i].key) {
        // 큰 요소들 한 칸씩 뒤로 밈. 
        a[i+1] = a[i];
        i--;
    }
    // 멈춘 요소보다 한 칸 뒤에 위치해야 하므로, i+1에다가 저장
    a[i+1] = e;

}

void inesertionSort(element a[], int n) {
    int j;
    // 배열의 2번째 요소부터 정렬 시작
    for(j=2;j<=n;j++) {
        element temp = a[j];
        // 해당 요소를 a의 배열 안에서 요소 바로 앞 요소부터 비교하며 정렬 시작
        insert(temp, a, j-1);
    }
}
