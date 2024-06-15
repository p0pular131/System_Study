#include <stdio.h>

typedef struct{
    int key;
    int data;
}element;

void bubbleSort(element a[], int n) {
    // 배열의 크기를 하나씩 줄이면서 가장 큰 요소를 뒤로 빼는 정렬 방법
    // 최대값을 찾고 옮기는게 아니고, 주변 값들을 계속적으로 비교하며 정렬하는 알고리즘
    element tmp;
    int i,j;
    for(i=0;i<n-1;i++) { // 배열의 크기를 1개씩 점점 줄여나감
        for(j=0;j<n-1-i;j++) { // index 0인 것부터 주변 값들을 비교해나가며 정렬 시작
            if(a[j+1].key < a[j].key){ // 만약 현재값이 뒷값보다 크면 뒤로 현재 값을 뒤로 옮김
                tmp = a[j];
                a[j] = a[j+1];
                a[j+1] = tmp;
            }
        }
    }
}