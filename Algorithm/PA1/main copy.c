#include <stdio.h>
#include <stdlib.h>

typedef struct MinMaxHeap {
    int* data;
    int size;
    int capacity;
} MinMaxHeap;

// Min-Max Heap을 초기화하는 함수
MinMaxHeap* createHeap(int capacity) {
    MinMaxHeap* heap = (MinMaxHeap*)malloc(sizeof(MinMaxHeap));
    heap->data = (int*)malloc(sizeof(int) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

// Swap 함수
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify에서 최대 수준 유지
void maxHeapify(MinMaxHeap* heap, int idx) {
    int largest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->data[left] > heap->data[largest])
        largest = left;
    if (right < heap->size && heap->data[right] > heap->data[largest])
        largest = right;

    if (largest != idx) {
        swap(&heap->data[idx], &heap->data[largest]);
        maxHeapify(heap, largest);
    }
}

// Heapify에서 최소 수준 유지
void minHeapify(MinMaxHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->data[left] < heap->data[smallest])
        smallest = left;
    if (right < heap->size && heap->data[right] < heap->data[smallest])
        smallest = right;

    if (smallest != idx) {
        swap(&heap->data[idx], &heap->data[smallest]);
        minHeapify(heap, smallest);
    }
}

// Insert 함수
void insert(MinMaxHeap* heap, int value) {
    if (heap->size >= heap->capacity) {
        printf("Heap is full\n");
        return;
    }

    heap->data[heap->size] = value;
    heap->size++;

    // Max-Heap 성질 유지
    for (int i = (heap->size - 1) / 2; i >= 0; i--) {
        maxHeapify(heap, i);
    }
}

// Maximum 값 반환
int find_max(MinMaxHeap* heap) {
    if (heap->size > 0)
        return heap->data[0];
    return -1; // NULL을 표현
}

// Minimum 값 반환
int find_min(MinMaxHeap* heap) {
    if (heap->size > 0)
        return heap->data[1]; // 최소값은 루트의 첫 번째 자식
    return -1; // NULL을 표현
}

// 중앙값 찾기
int find_median(MinMaxHeap* heap) {
    if (heap->size == 0)
        return -1;

    if (heap->size % 2 == 1) {
        return heap->data[heap->size / 2];
    } else {
        int mid1 = heap->data[(heap->size - 1) / 2];
        int mid2 = heap->data[heap->size / 2];
        return (mid1 + mid2) / 2; // 평균을 반환
    }
}

int delete_max(MinMaxHeap* heap) {
    if (heap->size == 0) {
        printf("Heap is empty\n");
        return -1; // NULL을 표현
    }

    int max = heap->data[0]; // 루트 노드의 최대값 저장
    heap->data[0] = heap->data[heap->size - 1]; // 마지막 노드를 루트로 이동
    heap->size--; // 힙의 크기 감소

    maxHeapify(heap, 0); // 최대 힙 성질을 유지
    return max;
}

int delete_min(MinMaxHeap* heap) {
    if (heap->size < 2) {
        printf("Heap is empty or has only one element\n");
        return -1; // NULL을 표현
    }

    int min = heap->data[1]; // 루트의 첫 번째 자식 노드의 최소값 저장
    heap->data[1] = heap->data[heap->size - 1]; // 마지막 노드를 그 자리에 이동
    heap->size--; // 힙의 크기 감소

    minHeapify(heap, 1); // 최소 힙 성질을 유지
    return min;
}

int delete_median(MinMaxHeap* maxHeap, MinMaxHeap* minHeap) {
    if (maxHeap->size == 0 && minHeap->size == 0) {
        printf("Heap is empty\n");
        return -1; // NULL을 표현
    }

    int median;
    if (maxHeap->size > minHeap->size) {
        // 중앙값은 최대 힙의 루트
        median = deleteMax(maxHeap);
    } else {
        // 중앙값은 최소 힙의 루트
        median = deleteMin(minHeap);
    }

    // 힙의 균형을 유지합니다.
    balance(maxHeap, minHeap);
    return median;
}

void balance(MinMaxHeap* maxHeap, MinMaxHeap* minHeap) {
    while (maxHeap->size > minHeap->size + 1) {
        int max = deleteMax(maxHeap);
        insert(minHeap, max);
    }
    while (minHeap->size > maxHeap->size) {
        int min = deleteMin(minHeap);
        insert(maxHeap, min);
    }
}

// Main 함수
int main() {
    int capacity = 100;
    MinMaxHeap* heap = createHeap(capacity);
    FILE *input = fopen("my.in", "r"); 
    FILE *output = fopen("my.out", "w"); 

    int N; // the number of operations
    char operation[2], operand[2]; // current operation type
    int inputNumber; // current input number for insert
    int *maxHeap, *minHeap, *symmHeap; // pointers of heap
    fscanf(input,"%d",&N);
    maxHeap = (int*)malloc(sizeof(int)*(N/2+1)); // we can assume that the number of insertion is less than N
    minHeap = (int*)malloc(sizeof(int)*(N/2+1));

    while(N-- > 0) {
        char temp[4];
        fscanf(input,"%s",operation);
        if(operation[0] == 'I') { 
        /*
            1. insert the input value to short heap. 
               -> if sizes are same, input to maxHeap. 
            2. heapify each heaps
            3. change the root value if maxHeap's root is laerger than minHeap's. 
        */
            fscanf(input,"%d",&inputNumber);
            insert(heap,inputNumber);
        }
        else if(operation[0] == 'D') {
            fscanf(input, "%s", operand);
            if(operand[0] == 'M') {
                int max = delete_max(maxHeap);
            }
            else if(operand[0] == 'X') {
                int min = delete_min(minHeap);
            }
            else if(operand[0] == 'E') {
                int median = delete_median(maxHeap,minHeap);
            }
            balance(maxHeap, minHeap);
        }
        else if(operation[0] == 'F'){
            fscanf(input, "%s", operand);
            if(operand[0] == 'M') {
                int max = find_max(maxHeap,minHeap);
                if(max == -1) {
                    fprintf(output,"NULL\n");
                }
                else {
                    fprintf(output,"%d\n",max);
                }
            }
            else if(operand[0] == 'X') {
                int min = find_min(maxHeap,minHeap);
                if(min == -1) {
                    fprintf(output,"NULL\n");
                }
                else {
                    fprintf(output,"%d\n",min);
                }
            }
            else if(operand[0] == 'E') {
                int median = find_median(maxHeap,minHeap);
                if(median == -1) {
                    fprintf(output,"NULL\n");
                }
                else {
                    fprintf(output,"%d\n",median);
                }
            }
        }
    }

    free(heap->data);
    free(heap);
    return 0;
}
