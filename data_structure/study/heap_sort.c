#include <stdio.h>
#define SWAP(a, b, c) {c=a; a=b; b=c;}

typedef struct {
    int key;
    int data;
}element;

void adjust(element list[], int root, int n){  
    // 주어진 크기만큼의 배열 안에서 max-heap을 만들어주는 함수
    int child, rootkey;
    element temp;
    temp = list[root];
    rootkey = list[root].key;
    child = 2*root;/*leftchild*/
    // max-heap 만드는 반복문
    while(child <= n){
        // 왼쪽, 오른쪽 child를 비교해서 큰 것으로 child update
        if(child < n && list[child].key < list[child+1].key)
            child++;
        // root가 더 크면 종료
        if(rootkey > list[child].key)
            break;
        // root가 작으면 root 자리에 child를 올리고 밑의 child로 이동
        else{
            list[child/2] = list[child];
            child *= 2;
        }/*else*/
    }/*while*/
    // 종료된 위치에 원래 root를 이동
    list[child/2] = temp;
}


void heapsort(element list[], int n){
    /*
    max-heap을 만든 후 최상단 node를 배열의 맨 뒤로 빼는 과정을 반복
    heap의 크기를 하나씩 줄여나가면 결국 제일 큰 값을 1번씩 찾아나가게 됨
    -> 결국 배열의 맨 뒤부터 가장 큰 값들이 하나씩 자리하면서 정렬되는 알고리즘
    */
    int i,j;
    element temp;
    for(i=n/2;i>0;i--)
    // 트리의 중간부터 시작해서 max heap 생성
        adjust(list, i, n);
    for(i=n-1;i>0;i--) {
        // root를 맨 뒤로 빼고
        SWAP(list[1],list[i+1],temp);
        // 힙의 크기를 1개 줄여서 다시 max-heap 구성
        adjust(list,1,i);
    }
}