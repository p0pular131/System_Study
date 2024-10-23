#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

float poly[301][3];
float weights[301];
float P[3];
int polSize = 0;

int update_P() {
    if(polSize < 1) {
        return -1;
    }
    float weightSum = 0.0f;
    for(int i=1;i<=polSize;i++) {
        weightSum += weights[i];
    }
    
    return 0;
}


void add_function(float a, float b, float c) {
    poly[polSize+1][2] = a; // 2차항 계수
    poly[polSize+1][1] = b; // 1차항 계수
    poly[polSize+1][0] = c; // 상수
    polSize++; 
}

int remove_function(int idx) {
    if(idx > polSize) {
        return -1;
    }
    for(int i=idx;i<polSize;i++) {
        // move polynomials
        poly[idx][0] = poly[idx+1][0];
        poly[idx][1] = poly[idx+1][1];
        poly[idx][2] = poly[idx+1][2];
    }
    for(int i=idx;i<polSize;i++) {
        // move weights
        weights[idx] = weights[idx+1];
    }
    polSize--;
    return 0;
}

int set_weight(int idx, float weight) {
    if(idx > polSize) {
        return -1;
    }
    weights[idx] = weight;
    return 0;
}

double evaluate_prime(float x) {
    double result = 0.0;
    for(int idx = 0; idx < 3; idx ++) {
        P[idx] = 0;
        for(int i=1; i<=polSize; i++) {
            
        }
    }
    return result;
}

void find_root() {

}

int main()
{
    char command[50];
    
    for(int i=0; i<301; i++) {
        // initialize weights '1.0'
        weights[i] = 1.0f;
    }

    while (1)
    {
        scanf("%s", command);

        if (strcmp(command, "add") == 0)
        {
            float a, b, c;
            scanf("%f %f %f", &a, &b, &c);
            add_function(a, b, c);
        }
        else if (strcmp(command, "remove") == 0)
        {
            int index;
            scanf("%d", &index);
            printf("%d\n", remove_function(index));
        }
        else if (strcmp(command, "setw") == 0)
        {
            int index;
            float weight;
            scanf("%d %f", &index, &weight);
            printf("%d\n", set_weight(index, weight));
        }
        else if (strcmp(command, "evaluate") == 0)
        {
            float x;
            scanf("%f", &x);
            printf("%.3f\n", evaluate_prime(x));
        }
        else if (strcmp(command, "findroot") == 0)
        {
            find_root();
        }
        else if (strcmp(command, "exit") == 0)
        {
            break;
        }
    }

    return 0;
}
