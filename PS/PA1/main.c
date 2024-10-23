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

int check_zero(float input) {
    if(input < 0.01 && input > -0.01) {
        return 1;
    }
    return -1;
}

int update_P() {
    if(polSize < 1) {
        return -1;
    }
    float weightSum = 0.0f;
    for(int i=1;i<=polSize;i++) {
        weightSum += weights[i];
    }
    P[0] = 0.0f; P[1] = 0.0f; P[2] = 0.0f;
    for(int i=1;i<=polSize;i++) {
        P[0] += weights[i] * poly[i][0];
        P[1] += weights[i] * poly[i][1];
        P[2] += weights[i] * poly[i][2];
    }
    P[0] /= weightSum;
    P[1] /= weightSum;
    P[2] /= weightSum;
    return 0;
}

void add_function(float a, float b, float c) {
    poly[polSize+1][2] = a; // 2차항 계수
    poly[polSize+1][1] = b; // 1차항 계수
    poly[polSize+1][0] = c; // 상수
    polSize++; 
}

int remove_function(int idx) {
    if(idx > polSize || idx < 1) {
        return -1;
    }
    for(int i=idx;i<polSize;i++) {
        // move polynomials
        poly[i][0] = poly[i+1][0];
        poly[i][1] = poly[i+1][1];
        poly[i][2] = poly[i+1][2];
    }
    for(int i=idx;i<polSize;i++) {
        // move weights
        weights[i] = weights[i+1];
    }
    polSize--;
    return 0;
}

int set_weight(int idx, float weight) {
    if(idx > polSize || idx < 1) {
        return -1;
    }
    weights[idx] = weight;
    return 0;
}

float evaluate_prime(float x) {
    update_P(); 
    return P[0] + P[1]*x + P[2]*x*x;;
}

void find_root() {
    update_P();

    if(check_zero(P[2])==1 && check_zero(P[1])==1) {
        printf("CONST\n");
    }
    else if(check_zero(P[2])==1 && check_zero(P[1])==-1) {
        printf("%.3f\n", -P[0]/P[1]);
    }
    else {
        float det = P[1]*P[1] - 4*P[2]*P[0];
        if(check_zero(det)==1) {
            printf("%.3f\n", -P[1]/(2*P[2]));
        }
        else if(det > 0) {
            printf("%.3f %.3f\n", (-P[1] - sqrt(det))/(2*P[2]), (-P[1] + sqrt(det))/(2*P[2]));
        }       
        else {
            printf("NOROOT\n");
        }
    }
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
