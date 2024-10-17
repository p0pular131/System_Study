#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int maxHeapLength = 0, minHeapLength = 0; // global variable for counting the current length of heaps
int maxHeap[500000], minHeap[500000];

void insert(int element); // insert an integer element into the prioruty queue

int delete_min(); // deletes and returns the minimum element

int delete_max(); // deletes and returns the maximum element

int delete_median(); // deletes and returns the median element

int find_min(); // fetches the minimum element and print it

int find_max(); // fetches the maximum element and print it

int find_median(); // fetches the median element and print it

int main() {
/*
    first input: the number of operations to perform (<=500,000)

    input commands: <command> <number> 
            I: insert          D: delete            F: find
            M: minimum         X: maximum           E: mdedian      
*/
    int N; // the number of operations
    char operation; // current operation type
    char operand1, operand2; // current operand 
    scanf("%d",&N);

    while(N-- > 0) {
        scanf("%c %c", &operand1, &operand2);
        if(operand1 == 'I') { 
        /*
            1. insert the input value to short heap. 
               -> if sizes are same, input to maxHeap. 
            2. sort each heaps
            3. change the root value if maxHeap's root is laerger than minHeap's. 
            4. sort again
        */
            int inputNumber = operand2 - '0';
            insert(inputNumber);
        }
        else if(operand1 == 'D') {
            if(operand2 == 'M') {

            }
            else if(operand2 == 'X') {

            }
            else if(operand2 == 'E') {

            }
        }
        else if(operand1 == 'F'){
            if(operand2 == 'M') {

            }
            else if(operand2 == 'X') {

            }
            else if(operand2 == 'E') {
                
            }
        }
    }

    return 0;
}