#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int maxHeapLength = 0, minHeapLength = 0; // global variable for counting the current length of heaps

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
} 

void printHeap(int* maxheap, int* minheap) {
    // print function for debug
    printf("Maxheap length : %d\n",maxHeapLength);
    for(int i = 0; i < maxHeapLength; i++) {
        printf("%d ",maxheap[i]);
    }
    printf("\n");

    printf("Minheap length : %d\n",minHeapLength);
    for(int i = 0; i < minHeapLength; i++) {
        printf("%d ",minheap[i]);
    }
    printf("\n");
}

void heapify_up(int* maxHeap, int* minHeap) {
    // 1. heapify maxheap upward
    int idx = maxHeapLength - 1;
    if(idx != 0) {
        while((idx!= 0) && (maxHeap[idx] > maxHeap[(idx-1)/2])) {
            int parent = (idx - 1) / 2; // parent node
            swap(&maxHeap[idx], &maxHeap[parent]);
            idx = parent;
        }
    }

    // 2. heapify minheap downward
    idx = minHeapLength - 1;
    if(idx != 0) {
        while((idx!= 0) && (minHeap[idx] < minHeap[(idx-1)/2])) {
            int parent = (idx - 1) / 2; // parent node
            swap(&minHeap[idx], &minHeap[parent]);
            idx = parent;
        }
    }

    // 3. swap the root of maxHeap and minHeap if maxHeap's root is larger than minHeap's root 
    if(maxHeap[0] > minHeap[0] && maxHeapLength > 0 && minHeapLength > 0) {
        swap(&maxHeap[0], &minHeap[0]);
    }

}

void heapify_up_delete(int* heap, int heapLength, int startIdx, int minmax) {
    // heapify upeard starting at deleted element's location
    int idx = startIdx;
    // minmax : 0 -> minheap, 1 -> maxheap
    if(minmax == 0){
        if(heapLength != 0) {
            while((idx!= 0) && (heap[idx] > heap[(idx-1)/2])) {
                int parent = (idx - 1) / 2; // parent node
                swap(&heap[idx], &heap[parent]);
                idx = parent;
            }
        }
    }
    // 1 -> maxheap 
    else {
        if(heapLength != 0) {
            while((idx!= 0) && (heap[idx] < heap[(idx-1)/2])) {
                int parent = (idx - 1) / 2; // parent node
                swap(&heap[idx], &heap[parent]);
                idx = parent;
            }
        }
    }
}


void heapify_down(int* maxHeap, int* minHeap) {
    // 1. heapify maxheap downward
    int idx = 0;
    if(maxHeapLength -1 != 0) {
        while((2*idx + 1 < maxHeapLength)) {
            int leftChild = 2 * idx + 1;
            int rightChild = 2 * idx + 2;
            int largest = idx;

            if(leftChild < maxHeapLength && maxHeap[leftChild] > maxHeap[largest]) {
                largest = leftChild;
            }
            if(rightChild < maxHeapLength && maxHeap[rightChild] > maxHeap[largest]) {
                largest = rightChild;
            }
            if(largest != idx) {
                swap(&maxHeap[idx], &maxHeap[largest]);
                idx = largest;
            } 
            else 
            {
                break;
            }
        }
    }

    // 2. heapify maxheap downward
    idx = 0;
    if(minHeapLength -1 != 0) {
        while((2*idx + 1 < minHeapLength)) {
            int leftChild = 2 * idx + 1;
            int rightChild = 2 * idx + 2;
            int smallest = idx;

            if(leftChild < minHeapLength && minHeap[leftChild] < minHeap[smallest]) {
                smallest = leftChild;
            }
            if(rightChild < minHeapLength && minHeap[rightChild] < minHeap[smallest]) {
                smallest = rightChild;
            }
            if(smallest != idx) {
                swap(&minHeap[idx], &minHeap[smallest]);
                idx = smallest;
            } 
            else 
            {
                break;
            }
        }
    }
    // 3. swap the root of maxHeap and minHeap if maxHeap's root is larger than minHeap's root 
    if(maxHeap[0] > minHeap[0] && maxHeapLength > 0 && minHeapLength > 0) {
        swap(&maxHeap[0], &minHeap[0]);
    }
}


void insert(int* maxHeap, int* minHeap, int element) {
    // if maxHeap is empty, insert the elemet to the maxHeap first. 
    if(maxHeapLength == 0) {
        maxHeap[0] = element;
        maxHeapLength++;
    }
    else if(minHeapLength == 0) {
        minHeap[0] = element;
        minHeapLength++;
    }
    // if both heaps are not empty, compare the length of each heap and insert to smaller one. 
    else {
        if(maxHeapLength <= minHeapLength) {
            maxHeap[maxHeapLength] = element;
            maxHeapLength++;
        }
        else if(maxHeapLength > minHeapLength) {
            minHeap[minHeapLength] = element;
            minHeapLength++;
        }
    }
    // heapify again
    heapify_up(maxHeap, minHeap);
    heapify_down(maxHeap, minHeap);
}


int find_min(int* maxHeap, int* minHeap) {
    int result = __INT32_MAX__, resultIdx = 0;
    if(maxHeapLength==0) {
        if(minHeapLength == 0) {
            // no value in heaps
            return -1;
        }
        else {
            // only minHeap has one element
            return minHeap[0];
        }
    }
    else if(minHeapLength == 0) {
        // only maxHeap has one element
        return maxHeap[0];
    }
    // find minimum value in maxHeap 
    for(int i = 0; i < maxHeapLength; i++) {
        if(maxHeap[i] < result) {
            result = maxHeap[i];
            resultIdx = i;
        }
    }
    return result;
}

int find_max(int* maxHeap, int* minHeap) {
    int result = -__INT32_MAX__-1, resultIdx = 0;
    if(maxHeapLength==0) {
        if(minHeapLength == 0) {
            // no value in heaps
            return -1;
        }
        else {
            // only minHeap has one element
            return minHeap[0];
        }
    }
    else if(minHeapLength == 0) {
        // only maxHeap has one element
        return maxHeap[0];
    }
    // find maximum value in minHeap
    for(int i=0; i< minHeapLength; i++) {
        if(minHeap[i] > result) {
            result = minHeap[i];
            resultIdx = i;
        }
    }
    return result;
}   

int find_median(int* maxHeap, int* minHeap) {
    if(maxHeapLength==0) {
        if(minHeapLength == 0) {
            // no value in heaps
            return -1;
        }
        else {
            // only minHeap has one element
            return minHeap[0];
        }
    }
    else if(minHeapLength == 0) {
        // only maxHeap has one element
        return maxHeap[0];
    }
    // if lengths are same, smaller one is median.
    if(maxHeapLength == minHeapLength) {
        return maxHeap[0] < minHeap[0] ? maxHeap[0] : minHeap[0];
    }
    else {
        // if lengths are different, root is always median
        return maxHeapLength > minHeapLength ? maxHeap[0] : minHeap[0];
    }
}

int delete_min(int* maxHeap, int* minHeap) {
    if(maxHeapLength == 0 && minHeapLength == 0) {
        return -1;
    }
    // 1. find minumum
    int result = __INT32_MAX__, resultIdx = 0;
    if(maxHeapLength==0) {
        result = minHeap[0];
        minHeapLength--;
        return result;
    }
    for(int i = 0; i < maxHeapLength; i++) {
        if(maxHeap[i] < result) {
            result = maxHeap[i];
            resultIdx = i;
        }
    }
    // 2. delete minimum
    if(resultIdx == maxHeapLength-1) {
        maxHeapLength--;
    }
    else {
        // swap the value with leaf node and delete
        swap(&maxHeap[maxHeapLength-1], &maxHeap[resultIdx]);
        maxHeapLength--;
        // and heapify
        heapify_up_delete(maxHeap, maxHeapLength, resultIdx, 1);
    }
    heapify_down(maxHeap, minHeap);
    return result;
}

int delete_max(int* maxHeap, int* minHeap) {
    if(maxHeapLength == 0 && minHeapLength == 0) {
        return -1;
    }
    // 1. find maximum
    int result = -__INT32_MAX__-1, resultIdx = 0;
    if(minHeapLength==0) {
        result = maxHeap[0];
        maxHeapLength--;
        return result;
    }
    for(int i=0; i< minHeapLength; i++) {
        if(minHeap[i] > result) {
            result = minHeap[i];
            resultIdx = i;
        }
    }
    // 2. delete maximum
    if(resultIdx == minHeapLength-1) {
        minHeapLength--;
    }
    else {
        // swap the value with leaf node and delete
        swap(&minHeap[minHeapLength-1], &minHeap[resultIdx]);
        minHeapLength--;
        // and heapify
        heapify_up_delete(minHeap, minHeapLength, resultIdx, 0);
    }
    return result;    
}

int delete_median(int* maxHeap, int* minHeap) {
    int result = 0;
    if(maxHeapLength == 0 && minHeapLength == 0) {
        result = -1;
        return result;
    }
    // 1. find median
    result = find_median(maxHeap, minHeap);
    // 2. swap root with leaf and heapify
    if(result == maxHeap[0]) {
        maxHeap[0] = maxHeap[maxHeapLength-1];
        maxHeapLength--;
        heapify_down(maxHeap, minHeap);
        if(maxHeapLength > 0 && minHeapLength > 0) {
            if(maxHeap[0] > minHeap[0]) {
                swap(&maxHeap[0], &minHeap[0]);
            }
        }
    }
    else {
        minHeap[0] = minHeap[minHeapLength-1];
        minHeapLength--;
        heapify_down(maxHeap, minHeap);
        if(maxHeapLength > 0 && minHeapLength > 0) {
            if(maxHeap[0] > minHeap[0]) {
                swap(&maxHeap[0], &minHeap[0]);
            }
        }
    }
    return result;
}

void balance(int* maxHeap, int* minHeap) {
    if(maxHeapLength == 0 && minHeapLength == 0) {
        return;
    }
    // we must check that diff of lengths are not above 2. 
    // if it above 2, we have to move the longger heap's root to the shorter heap's leaf. 
    if(fabs(maxHeapLength - minHeapLength) >= 2) {
        if(maxHeapLength > minHeapLength) {
            // take the root of maxHeap to minHeap
            minHeap[minHeapLength] = maxHeap[0];
            minHeapLength++;
            heapify_up(maxHeap, minHeap);
            // delete the root of maxHeap
            maxHeap[0] = maxHeap[maxHeapLength-1];
            maxHeapLength--;
            heapify_down(maxHeap, minHeap);
        }
        else {
            // take the root of maxHeap to minHeap
            maxHeap[maxHeapLength] = minHeap[0];
            maxHeapLength++;
            heapify_up(maxHeap, minHeap);
            // delete the root of minHeap
            minHeap[0] = minHeap[minHeapLength-1];
            minHeapLength--;
            heapify_down(maxHeap, minHeap);
        }
    }
}

int main() {
/*
    first input: the number of operations to perform (<=500,000)

    input commands: <command> <number> 
            I: insert          D: delete            F: find
            M: minimum         X: maximum           E: mdedian      
*/
    FILE *input = fopen("pq.in", "r"); 
    FILE *output = fopen("pq.out", "w"); 

    int N; // the number of operations
    char operation[2], operand[2]; // current operation type
    int inputNumber; // current input number for insert
    int *maxHeap, *minHeap; // pointers of heap
    fscanf(input,"%d",&N);
    maxHeap = (int*)malloc(sizeof(int)*(N/2+1)); // we can assume that the number of insertion is less than N
    minHeap = (int*)malloc(sizeof(int)*(N/2+1));

    while(N-- > 0) {
        char temp[4];
        fscanf(input,"%s",operation);
        if(operation[0] == 'I') { 
            fscanf(input,"%d",&inputNumber);
            insert(maxHeap,minHeap,inputNumber);
        }
        else if(operation[0] == 'D') {
            fscanf(input, "%s", operand);
            if(operand[0] == 'M') {
                int min = delete_min(maxHeap, minHeap);
            }
            else if(operand[0] == 'X') {
                int max = delete_max(minHeap, minHeap);
            }
            else if(operand[0] == 'E') {
                int median = delete_median(maxHeap,minHeap);
            }
            // check difference of two heap's length 
            // because the delete operation may cause the large difference of lenghts
            balance(maxHeap, minHeap);
        }
        else if(operation[0] == 'F'){
            fscanf(input, "%s", operand);
            if(operand[0] == 'M') {
                int min = find_min(maxHeap,minHeap);
                if(min == -1) {
                    fprintf(output,"NULL\n");
                }
                else {
                    fprintf(output,"%d\n",min);
                }
            }
            else if(operand[0] == 'X') {
                int max = find_max(maxHeap,minHeap);
                if(max == -1) {
                    fprintf(output,"NULL\n");
                }
                else {
                    fprintf(output,"%d\n",max);
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

    free(maxHeap);
    free(minHeap);
    fclose(input);
    fclose(output);

    return 0;
}