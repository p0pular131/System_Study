#include <stdio.h>

void selectionSort(int a[], int n) {
    int i, j, min, temp;
    for(i=0;i<n-1;i++) {
        /*
        가장 작은 값을 앞으로 빼는 알고리즘
        i를 하나씩 늘려가며 해당 배열의 최소값을 찾은 다음에 앞으로 뺌. 
        */
        min = i;
        for(j=i+1;j<n;j++) {
            if(a[j]<a[min]) min = j; // 최소값 찾는 과정
        }
        // 찾았으면 swap
        temp = a[i];
        a[i] = a[min];
        a[min] = temp;
    }
}