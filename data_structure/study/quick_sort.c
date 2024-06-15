#include <stdio.h>
#define SWAP(a, b, c) {c=a; a=b; b=c;}

int count = 0;

void quicksort(int[], int, int);

int main() { 
    int a[10] = {26, 5, 37, 1, 61, 11, 59, 15, 48, 19};
    int i;
    quicksort(a,0,9);

    return 0;
}

void quicksort(int a[], int left, int right) {
    int pivot, i, j, temp;
    count++;
    printf("count = %d: left = %d, right = %d\n", count, left, right);
    for(i=0;i<10;i++) printf("%d ", a[i]);
    printf("\n");

    if (left < right) {
        // pivot의 인덱스 
        i = left;
        // do-while문이라서 right+1로 최초 j 정의
        j = right+1;
        pivot = a[left];
        do {
            do i++; while(a[i]<pivot);
            do j--; while(a[j]>pivot);
            if(i<j) SWAP(a[i],a[j],temp);
        } while(i<j);
        SWAP(a[left], a[j], temp);
        printf("(%d,%d) \n",i,j);
        quicksort(a,left,j-1);
        quicksort(a,j+1,right);
    }
}