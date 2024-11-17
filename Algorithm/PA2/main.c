#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 150
#define MAX_COMB 5

typedef struct dna{
    char* sequence; // input dna suquence
    int length;     // length of the sequence
    int** letter;   // A, T, G, C의 index 저장
} DNA;

DNA dna[MAX_COMB];  // input dna sequence 저장
char* result_lcs;
int num_comb;

void input_dna(FILE* input) {
    for (int i = 0; i < num_comb; i++) {
        dna[i].sequence = malloc(sizeof(char) * MAX_LENGTH);
        fscanf(input, "%s", dna[i].sequence);
        dna[i].length = strlen(dna[i].sequence);

        // 각 위치에 문자(A, T, G, C)가 나오는 인덱스를 저장할 배열 초기화
        int a = -1, t = -1, g = -1, c = -1;
        dna[i].letter = malloc(sizeof(int*) * 4); // 4개의 문자(A, T, G, C)에 대한 배열

        for (int j = 0; j < 4; j++) {
            dna[i].letter[j] = malloc(sizeof(int) * MAX_LENGTH); // j 번째 문자의 위치 저장
            for (int k = 0; k < MAX_LENGTH; k++) {
                dna[i].letter[j][k] = -1; // 초기화
            }
        }
        for (int j = dna[i].length - 1; j >= 0; j--) {
            if (dna[i].sequence[j] == 'A') {
                dna[i].letter[0][j] = j + 1; // A의 현재 위치
                dna[i].letter[1][j] = t;     // T의 다음 위치
                dna[i].letter[2][j] = g;     // G의 다음 위치
                dna[i].letter[3][j] = c;     // C의 다음 위치
                a = j + 1;
            } else if (dna[i].sequence[j] == 'T') {
                dna[i].letter[0][j] = a;
                dna[i].letter[1][j] = j + 1;
                dna[i].letter[2][j] = g;
                dna[i].letter[3][j] = c;
                t = j + 1;
            } else if (dna[i].sequence[j] == 'G') {
                dna[i].letter[0][j] = a;
                dna[i].letter[1][j] = t;
                dna[i].letter[2][j] = j + 1;
                dna[i].letter[3][j] = c;
                g = j + 1;
            } else if (dna[i].sequence[j] == 'C') {
                dna[i].letter[0][j] = a;
                dna[i].letter[1][j] = t;
                dna[i].letter[2][j] = g;
                dna[i].letter[3][j] = j + 1;
                c = j + 1;
            }
        }
    }
}

void lcs_2() {
    // 2D dp array initialze
    int** dp = (int**)malloc(sizeof(int*) * (dna[0].length+1));
    for(int i=0;i<dna[0].length+1;i++) {
        dp[i] = (int*)malloc(sizeof(int) * (dna[1].length+1));
    }   
    // make dp dp
    for(int i=0;i<dna[0].length+1;i++) {
        for(int j=0;j<dna[1].length+1;j++) {
            if(i == 0 || j == 0) {
                dp[i][j] = 0;
            } 
            else if(dna[0].sequence[i-1] == dna[1].sequence[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
            } 
            else {
                dp[i][j] = (dp[i-1][j] > dp[i][j-1]) ? dp[i-1][j] : dp[i][j-1];
            }
        }
    }
    // bactracking
    int lcs_leng = dp[dna[0].length][dna[1].length];
    result_lcs = malloc(sizeof(char) * (lcs_leng));
    result_lcs[lcs_leng] = '\0';

    int idx = lcs_leng-1;
    int i = dna[0].length; int j = dna[1].length;
    while(i > 0 && j > 0){
        if(dp[i-1][j] == dp[i][j]){
            i--;
        }
        else if(dp[i][j-1] == dp[i][j]){
            j--;
        }
        else{
            result_lcs[idx] = dna[0].sequence[i-1];
            i--; j--;
            idx--;
        }
    }
    free(dp);
}

void lcs_3() {
    // 3D array initialize
    int*** dp = (int***)malloc(sizeof(int*) * (dna[0].length+1));
    for(int i=0;i<dna[0].length+1;i++) {
        dp[i] = (int**)malloc(sizeof(int*) * (dna[1].length+1));
        for(int j=0;j<dna[1].length+1;j++) {
            dp[i][j] = (int*)malloc(sizeof(int) * (dna[2].length+1));
        }
    }
    // make dp dp
    for(int i=0;i<dna[0].length+1;i++) {
        for(int j=0;j<dna[1].length+1;j++) {
            for(int k=0;k<dna[2].length+1;k++) {
                if(i == 0 || j == 0 || k == 0) {
                    dp[i][j][k] = 0;
                } 
                else if(dna[0].sequence[i-1] == dna[1].sequence[j-1] && dna[1].sequence[j-1] == dna[2].sequence[k-1]) {
                    dp[i][j][k] = dp[i-1][j-1][k-1] + 1;
                } 
                else {
                    int max = dp[i-1][j][k];
                    if(dp[i][j-1][k] > max) {
                        max = dp[i][j-1][k];
                    }
                    if(dp[i][j][k-1] > max) {
                        max = dp[i][j][k-1];
                    }
                    dp[i][j][k] = max;
                }
            }
        }
    }
    // backtracking
    int lcs_leng = dp[dna[0].length][dna[1].length][dna[2].length];
    result_lcs = malloc(sizeof(char) * (lcs_leng));
    result_lcs[lcs_leng] = '\0';

    int idx = lcs_leng-1;
    int i = dna[0].length; int j = dna[1].length; int k = dna[2].length;
    while(i > 0 && j > 0 && k > 0){
        if(dp[i-1][j][k] == dp[i][j][k]){
            i--;
        }
        else if(dp[i][j-1][k] == dp[i][j][k]){
            j--;
        }
        else if(dp[i][j][k-1] == dp[i][j][k]){
            k--;
        }
        else{
            result_lcs[idx] = dna[0].sequence[i-1];
            i--; j--; k--;
            idx--;
        }
    }
    free(dp);
}

// Function to initialize a 4D array
int**** init_4d_array(int size) {
    int**** dp = malloc(sizeof(int***) * size);
    for (int i = 0; i < size; i++) {
        dp[i] = malloc(sizeof(int**) * size);
        for (int j = 0; j < size; j++) {
            dp[i][j] = malloc(sizeof(int*) * size);
            for (int k = 0; k < size; k++) {
                dp[i][j][k] = malloc(sizeof(int) * size);
                for (int l = 0; l < size; l++) {
                    dp[i][j][k][l] = -1; // Initialize with -1
                }
            }
        }
    }
    return dp;
}

// Function to free a 4D array
void free_4d_array(int**** dp, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                free(dp[i][j][k]);
            }
            free(dp[i][j]);
        }
        free(dp[i]);
    }
    free(dp);
}

// Function to calculate the LCS length using 4D DP
int calc_lcs_length_4(int**** dp, int* indexes) {
    // Check if indexes reach the end of any string
    for (int i = 0; i < num_comb; i++) {
        if (indexes[i] == dna[i].length) {
            return 0;
        }
    }

    // Check if the value is already computed in the dp
    int dp_value = dp[indexes[0]][indexes[1]][indexes[2]][indexes[3]];
    if (dp_value != -1) {
        return dp_value;
    }

    // Check if all characters match at current indexes
    char current_char = dna[0].sequence[indexes[0]];
    int all_match = 1;
    for (int i = 1; i < num_comb; i++) {
        if (dna[i].sequence[indexes[i]] != current_char) {
            all_match = 0;
            break;
        }
    }

    int max_length = 0;

    // Case 1: All characters match, move all indexes forward
    if (all_match) {
        int next_indexes[4];
        for (int i = 0; i < num_comb; i++) {
            next_indexes[i] = indexes[i] + 1;
        }
        max_length = calc_lcs_length_4(dp, next_indexes) + 1;
    }

    // Case 2: Move each index independently
    for (int i = 0; i < num_comb; i++) {
        int next_indexes[4];
        memcpy(next_indexes, indexes, sizeof(int) * num_comb);
        next_indexes[i]++;
        int length = calc_lcs_length_4(dp, next_indexes);
        if (length > max_length) {
            max_length = length;
        }
    }

    // Store the computed value in the dp
    dp[indexes[0]][indexes[1]][indexes[2]][indexes[3]] = max_length;
    return max_length;
}

// Backtracking to construct the LCS
void backtracking_4(int lcs_length, int**** dp) {
    result_lcs = malloc(sizeof(char) * (lcs_length + 1));
    result_lcs[lcs_length] = '\0';

    int indexes[4] = {0, 0, 0, 0}; // Start from the root (0,0,0,0)

    for (int i = 0; i < lcs_length; i++) {
        char current_char = '\0';

        // Check if all sequences match the current character
        for (int j = 0; j < num_comb; j++) {
            if (indexes[j] < dna[j].length) {
                current_char = dna[j].sequence[indexes[j]];
            } 
            else {
                current_char = '\0';
                break;
            }
        }

        if (current_char != '\0') {
            result_lcs[i] = current_char;

            // Move indexes forward for the matching character
            for (int j = 0; j < num_comb; j++) {
                indexes[j]++;
            }
        }
    }
}

// Main function for 4-sequence LCS computation
void lcs_4() {
    // Initialize the 4D DP array
    int**** dp = init_4d_array(MAX_LENGTH);

    // Starting indexes (all zeros)
    int starting_indexes[4] = {0, 0, 0, 0};

    // Calculate LCS length
    int lcs_length = calc_lcs_length_4(dp, starting_indexes);

    // Traceback to find the LCS string
    backtracking_4(lcs_length, dp);

    // Free memory
    free_4d_array(dp, MAX_LENGTH);
}

// Function to initialize a 5D array
int***** init_5d_array(int size) {
    int***** dp = malloc(sizeof(int****) * size);
    for (int i = 0; i < size; i++) {
        dp[i] = malloc(sizeof(int***) * size);
        for (int j = 0; j < size; j++) {
            dp[i][j] = malloc(sizeof(int**) * size);
            for (int k = 0; k < size; k++) {
                dp[i][j][k] = malloc(sizeof(int*) * size);
                for (int l = 0; l < size; l++) {
                    dp[i][j][k][l] = malloc(sizeof(int) * size);
                    for (int m = 0; m < size; m++) {
                        dp[i][j][k][l][m] = -1; // Initialize with -1
                    }
                }
            }
        }
    }
    return dp;
}

// Function to free a 5D array
void free_5d_array(int***** dp, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                for (int l = 0; l < size; l++) {
                    free(dp[i][j][k][l]);
                }
                free(dp[i][j][k]);
            }
            free(dp[i][j]);
        }
        free(dp[i]);
    }
    free(dp);
}

// Function to calculate the LCS length using 5D DP
int calc_lcs_length_5(int***** dp, int* indexes) {
    // Check if indexes reach the end of any string
    for (int i = 0; i < num_comb; i++) {
        if (indexes[i] == dna[i].length) {
            return 0;
        }
    }

    // Check if the value is already computed in the dp
    int cached_value = dp[indexes[0]][indexes[1]][indexes[2]][indexes[3]][indexes[4]];
    if (cached_value != -1) {
        return cached_value;
    }

    // Check if all characters match at current indexes
    char current_char = dna[0].sequence[indexes[0]];
    int all_match = 1;
    for (int i = 1; i < num_comb; i++) {
        if (dna[i].sequence[indexes[i]] != current_char) {
            all_match = 0;
            break;
        }
    }

    int max_length = 0;

    // Case 1: All characters match, move all indexes forward
    if (all_match) {
        int next_indexes[MAX_COMB];
        for (int i = 0; i < num_comb; i++) {
            next_indexes[i] = indexes[i] + 1;
        }
        max_length = calc_lcs_length_5(dp, next_indexes) + 1;
    }

    // Case 2: Move each index independently
    for (int i = 0; i < num_comb; i++) {
        int next_indexes[MAX_COMB];
        memcpy(next_indexes, indexes, sizeof(int) * num_comb);
        next_indexes[i]++;
        int length = calc_lcs_length_5(dp, next_indexes);
        if (length > max_length) {
            max_length = length;
        }
    }

    // Store the computed value in the dp and return it
    dp[indexes[0]][indexes[1]][indexes[2]][indexes[3]][indexes[4]] = max_length;
    return max_length;
}

// Backtracking to construct the LCS
void backtracking_5(int lcs_length, int***** dp) {
    result_lcs = malloc(sizeof(char) * (lcs_length + 1));
    result_lcs[lcs_length] = '\0';

    int indexes[MAX_COMB] = {0};

    for (int i = 0; i < lcs_length; i++) {
        char current_char = '\0';

        // Check if all sequences match the current character
        for (int j = 0; j < num_comb; j++) {
            if (indexes[j] < dna[j].length) {
                current_char = dna[j].sequence[indexes[j]];
            } 
            else {
                current_char = '\0';
                break;
            }
        }

        if (current_char != '\0') {
            result_lcs[i] = current_char;
            for (int j = 0; j < num_comb; j++) {
                indexes[j]++;
            }
        }
    }
}

// Main function for 5-sequence LCS computation
void lcs_5() {
    // Initialize the 5D DP array
    int***** dp = init_5d_array(MAX_LENGTH);

    // Starting indexes
    int starting_indexes[MAX_COMB] = {0};

    // Calculate LCS length
    int lcs_length = calc_lcs_length_5(dp, starting_indexes);

    // Traceback to find the LCS
    backtracking_5(lcs_length, dp);

    // Free memory
    free_5d_array(dp, MAX_LENGTH);
}

int is_same(int lcs_index, int indexes[]) {
    for (int i = 0; i < num_comb; i++) {
        if (dna[i].sequence[indexes[i]] != result_lcs[lcs_index]) {
            return 0;
        }
    }
    return 1;
}

void write_output() {
    int lcs_index = 0;
    char* aligned_dna[num_comb];
    int max_length = 0;
    int indexes[5] = {0};
    int aligned_index = 0;
    int loop = 1;

    for(int i = 0; i < num_comb; i++){
        aligned_dna[i] = malloc(sizeof(char) * 500);
        for (int j = 0; j < 500; j++) {
            aligned_dna[i][j] = '-';
        }
    }
    
    FILE *output = fopen("hw2_output.txt", "w");

    while(loop){
        for(int i = 0; i < num_comb; i++){
            if(dna[i].sequence[indexes[i]] != result_lcs[lcs_index]){
                aligned_dna[i][aligned_index] = dna[i].sequence[indexes[i]];
            }
            else if (result_lcs[lcs_index] == dna[i].sequence[indexes[i]] && is_same(lcs_index, indexes)){ 
                for(int j = 0; j < num_comb; j++) aligned_dna[j][aligned_index] = dna[j].sequence[indexes[j]];
                lcs_index++;
                break;
            }
            else{
                indexes[i]--; 
            } 
        }
        aligned_index++;
        for(int i = 0; i <num_comb; i++) indexes[i]++;
        
        for(int i = 0; i < num_comb; i++){
            if(indexes[i] >= dna[i].length) {
                loop = 0;
                break;
            }
        }
    }

    int longest_aligned_length = 0;
    for(int i = 0; i < num_comb; i++){
        if(indexes[i] < dna[i].length){
            int temp_aligned_index = aligned_index;
            if((dna[i].length - indexes[i]) > longest_aligned_length) longest_aligned_length = dna[i].length - indexes[i];
            for(int j = indexes[i]; j < dna[i].length; j++){
                aligned_dna[i][temp_aligned_index] = dna[i].sequence[j];
                temp_aligned_index++;
            }
        }
    }
    longest_aligned_length += aligned_index;

    for(int i = 0; i < num_comb; i++) {
        for(int j = 0; j < longest_aligned_length; j++) fputc(aligned_dna[i][j], output);
        fputc('\n', output);
    }

    lcs_index = 0;
    for(int j = 0; j < aligned_index; j++){
        if(lcs_index > (strlen(result_lcs))) break;
        else if(result_lcs[lcs_index] == aligned_dna[0][j]){
            fputc('*', output);
            lcs_index++;
        }else{
            fputc(' ', output);
        }
    }
    
    for(int i = 0; i < num_comb; i ++) free(aligned_dna[i]);
    fclose(output);
}

int main() {
    // input file읽어서 dna저장 
    FILE *input_file = fopen("hw2_input.txt", "r");
    fscanf(input_file, "%d", &num_comb);
    fscanf(input_file, "%*s"); // $제거
    input_dna(input_file);
    fclose(input_file);

    switch(num_comb){
        case 2:
            lcs_2();
            break;
        case 3:
            lcs_3();
            break;
        case 4:
            lcs_4();
            break;
        case 5:
            lcs_5();
            break;
        default:
            break;
    }

    write_output();
    for(int i = 0; i < num_comb; i++) free(dna[i].sequence);

    return 0;
}