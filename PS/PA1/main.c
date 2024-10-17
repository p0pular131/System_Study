#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

int main()
{
    char command[50];
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
