#include "hpfp.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h> // 제출할 때 삭제하기
#define exp_size 5
#define frac_size 10
#define bias 15
#define max_hp 65504

hpfp int_converter(int input){
    hpfp value_return;
    int result_hp = 0;
    int result[16];
    int E=0,exp=0;
    // 배열 초기화
    for(int i=0;i<16;i++) result[i] = 0;
    // 예외처리 
    if(input > max_hp) {
        return (hpfp)0b0111110000000000;
    }
    else if(input < -max_hp) {
        return (hpfp)0b1111110000000000;
    }
    else if(input == 0) return (hpfp)0x0000;
    // 음수면 부호를 배열에 저장한 후 양수로 변환
    else {
        if(input<0){
            result[0] = 1;
            input *= -1;
        }
        int temp = input, size = 0;
        // 2진수의 길이를 바탕으로 E값 계산
        while(temp>0) {
            temp /= 2;
            size ++;
        }
        int deci_to_bin[size];
        E = size-1;
        // 2진수 변환 후 배열에 저장
        int idx = 0;
        while(input>0){
            deci_to_bin[size-idx-1] = input%2;
            input /= 2;
            idx++;
        }
        // 먼저 exp를 계산하여 2진수 변환 후 결과 배열에 저장
        exp = E + bias;
        for(int i=5;i>=1;i--){
            result[i] = exp%2;
            exp /= 2;
        }
        // frac을 바로 저장, 크기는 원래 2진수보다 1 작음
        for(int i=0;i<size-1;i++) result[i+6] = deci_to_bin[i+1];
        // 2진수를 2byte에 저장
        for(int i=15,temp=1;i>=0;i--){
            result_hp += temp*result[i];
            temp *= 2;
        }
    } // 오버플로, 언더플로 대한 데이터 처리
    if(result_hp > max_hp){
        value_return = 0b0111110000000000;
    }
    else if (result_hp < -max_hp) {
        value_return = 0b1111110000000000;
    }
    else value_return = (hpfp)result_hp;
    return value_return;
}

int hpfp_to_int_converter(hpfp input){

}

hpfp float_converter(float input){
    hpfp value_return;
    int result_hp = 0;
    int result[16];
    int E=0,exp=0;
    // 초기화
    for(int i=0;i<16;i++) result[i] = 0;
    // 예외처리 
    if(input > max_hp) {
        return (hpfp)0b0111110000000000;
    }
    else if(input < -max_hp) {
        return (hpfp)0b1111110000000000;
    }
    else if(input == 0) return (hpfp)0;
    // 음수면 일단 부호처리 먼저 진행
    if(input<0) {
        result[0] = 1;
        input *= -1;
    }
    int input_int = (int)input;
    float input_float = input-input_int;
    // 소수부, 정수부를 각각 2진수로 변환하여 저장
    int temp = input_int, size_int = 0;
    while(temp>0) {
        size_int ++;
        temp/=2;
    }
    if(input_int == 0) size_int = 1; // 0.xxx 예외처리
    int arr_int[size_int], idx = 0;
    while(input_int>0) {
        arr_int[size_int-idx-1] = input_int%2;
        input_int/=2;
        idx++;
    } 
    if(input_int == 0) arr_int[0] = 0; // 0.xxx 예외처리
    int arr_frac[10];
    for(int i=0;i<10;i++){
        input_float *= 2;
        arr_frac[i] = (int)input_float;
        if(input_float>=1) input_float -= 1;
    }
    // 정수부의 크기에 따라 E값을 먼저 계산 그리고 exp 계산 후 저장.
    E = size_int - 1;
    // 정수부가 0인 경우 소수부를 앞으로 끌어와야 E값 계산 가능
    int pull = 0;
    if(size_int == 1 && arr_int[0]==0){
        E = -1;
        while(arr_frac[pull]!=1){
            E--;
            pull++;
        }
        pull++;
    }
    exp = E + bias;
    for(int i=5;i>=1;i--){
        result[i] = exp%2;
        exp /= 2;
    }
    // frac은 정수부 먼저 넣고 나머지에 넣는 방식 
    // round zero이므로, 뒤는 그냥 버림
    // 단, E가 -인 경우는 다르게 frac을 생성해줘야 함.
    if(E>=0){ // 일반적인 경우
        int i=0;
        for(;i<size_int-1;i++){
            result[i+6] = arr_int[i+1];
        }
        int f_i = 0;
        for(;i<10;i++){
            result[i+6] = arr_frac[f_i];
            f_i ++;
        }
    }
    else { // E가 음수여서 frac을 최초 1을 만나는 시점 그 뒤부터 저장해야 함.
        for(int i=pull;i<10;i++){
            result[6+i-pull] = arr_frac[i];
        }
    }
    // 완성된 2진수 10진수로 변환
    for(int j=15,temp=1;j>=0;j--){
        result_hp += result[j]*temp;
        temp *= 2;
    }
    // 결과값에 따른 예외처리 진행
    if(result_hp > max_hp){
        value_return = 0b0111110000000000;
    }
    else if (result_hp < -max_hp) {
        value_return = 0b1111110000000000;
    }
    else value_return = (hpfp)result_hp;
    return value_return;
}

float hpfp_to_float_converter(hpfp input){
    
}

hpfp addition_function(hpfp a, hpfp b){
    hpfp result;
    int S=0,exp=0,frac=0;
    int S_a, S_b, exp_a, exp_b, frac_a, frac_b;
    // 비트 연산으로 s, exp, frac 가져오기
    S_a = (a & 0x8000) >> 15;
    S_b = (b & 0x8000) >> 15;
    exp_a = (a & 0x7C00) >> 10;
    exp_b = (b & 0x7C00) >> 10;
    // frac가져올 때 exp가 0이 아니면 1 붙여줌
    if(exp_a==0) frac_a = (a & 0x03FF);
    else frac_a = ((a & 0x03FF) | 0x0400);
    if(exp_b==0) frac_b = (b & 0x03FF);
    else frac_b = ((b & 0x03FF) | 0x0400);
    /*
        a와 b의 값에 따라서 예외처리 진행
        +inf -> 0x7C00
        -inf -> 0xFC00
        NaN -> 0x7C01
    */
    if(S_a == 0 && exp_a == 0b11111 && frac_a==0) { // a -> +inf
        if(S_b == 0 && exp_b == 0b11111 && frac_b == 0) return 0x7C00; 
        else if(S_b == 1 && exp_b == 0b11111 && frac_b == 0) return 0x7C01;
        else if(exp_b == 0b11111 && frac_b != 0) return 0x7C01;
        else return 0x7C00;
    }
    else if(S_a == 1 && exp_a == 0b11111 && frac_a==0) { // a -> -inf
        if(S_b == 0 && exp_b == 0b11111 && frac_b == 0) return 0x7C01; 
        else if(S_b == 1 && exp_b == 0b11111 && frac_b == 0) return 0xFC00;
        else if(exp_b == 0b11111 && frac_b != 0) return 0x7C01;
        else return 0xFC00;
    }
    else if(exp_a == 0b11111 && frac_a != 0) { // a -> NaN
        return 0x7C01;
    }
    else{ // a -> normal value
        if(S_b == 0 && exp_b == 0b11111 && frac_b == 0) return 0x7C00; 
        else if(S_b == 1 && exp_b == 0b11111 && frac_b == 0) return 0xFC00;
        else if(exp_b == 0b11111 && frac_b != 0) return 0x7C01;
        else { // b -> normal value
        // 먼저 지수 맞춰줌
            if(exp_a > exp_b) {
                frac_b = frac_b >> (exp_a-exp_b);
                exp_b = exp_a;
            }
            else if(exp_a < exp_b) {
                frac_a = frac_a >> (exp_b-exp_a);
                exp_a = exp_b;
            }
        // 덧셈 진행
            if(S_a == S_b) { 
                S = S_a;
                exp = exp_a;
                frac = frac_a + frac_b;
                if(frac & 0x0800) { // 자리수 맞춰줌
                    exp++;
                    frac = frac >> 1;
                }

            }
            else { // 부호가 다를 경우, 더 큰 수의 부호를 따름
                if(frac_a > frac_b){
                    S = S_a;
                    exp = exp_a;
                    frac = frac_a - frac_b;
                    while(!(frac & 0x0400)) { // 자리수 맞춰줌
                        frac = frac << 1;
                        exp--;
                    }
                }
                else if(frac_a < frac_b) {
                    S = S_b;
                    exp = exp_b;
                    frac = frac_b-frac_a;
                    while(!(frac & 0x0400)) {
                        frac = frac << 1;
                        exp--;
                    }
                }
            }
        }
    }
    if(exp>=31){ // 오버플로 발생
        if(S == 0) return 0x7C00;
        else return 0xFC00;
    }
    return ((S << 15) & 0x8000) | ((exp << 10) & 0x7c00) | (frac & 0x03FF);
}

hpfp multiply_function(hpfp a, hpfp b){
    hpfp result;
    int S=0,exp=0,frac=0;
    int S_a, S_b, exp_a, exp_b, frac_a, frac_b, round = 0;
    // 비트 연산으로 s, exp, frac 가져오기
    S_a = (a & 0x8000) >> 15;
    S_b = (b & 0x8000) >> 15;
    exp_a = (a & 0x7C00) >> 10;
    exp_b = (b & 0x7C00) >> 10;
    // frac가져올 때 exp가 0이 아니면 1 붙여줌
    if(exp_a==0) frac_a = (a & 0x03FF);
    else frac_a = ((a & 0x03FF) | 0x0400);
    if(exp_b==0) frac_b = (b & 0x03FF);
    else frac_b = ((b & 0x03FF) | 0x0400);
    /*
        a와 b의 값에 따라서 예외처리 진행
        +inf -> 0x7C00
        -inf -> 0xFC00
        NaN -> 0x7C01
    */
    if(S_a == 0 && exp_a == 0b11111 && frac_a==0) { // a -> +inf
        if(S_b == 0 && exp_b == 0b11111 && frac_b == 0) return 0x7C00; 
        else if(S_b == 1 && exp_b == 0b11111 && frac_b == 0) return 0xFC00;
        else if(exp_b == 0b11111 && frac_b != 0) return 0x7C01;
        else if(exp_b == 0 && frac_b == 0) return 0; 
        else if(S_b == 1) return 0xFC00;
        else if(S_b == 0) return 0x7C00;
    }
    else if(S_a == 1 && exp_a == 0b11111 && frac_a==0) { // a -> -inf
        if(S_b == 0 && exp_b == 0b11111 && frac_b == 0) return 0xFC00; 
        else if(S_b == 1 && exp_b == 0b11111 && frac_b == 0) return 0x7C00;
        else if(exp_b == 0b11111 && frac_b != 0) return 0x7C01;
        else if(exp_b == 0 && frac_b == 0) return 0;
        else if(S_b == 1) return 0x7C00;
        else if(S_b == 0) return 0xFC00;
    }
    else if(exp_a == 0b11111 && frac_a != 0) { // a -> NaN
        return 0x7c01;
    }
    else if(exp_a == 0 && frac_a == 0){
        if(exp_b == 0b11111 && frac_b != 0) return 0x7C01;
        else return 0;
    }
    else{ // a -> normal value
        if(S_a == 0 && S_b == 0 && exp_b == 0b11111 && frac_b == 0) return 0x7C00; 
        else if(S_b == 1 && S_b == 0 && exp_b == 0b11111 && frac_b == 0) return 0xFC00; 
        else if(S_a == 0 && S_b == 1 && exp_b == 0b11111 && frac_b == 0) return 0xFC00;
        else if(S_a == 1 && S_b == 1 && exp_b == 0b11111 && frac_b == 0) return 0x7C00;
        else if(exp_b == 0b11111 && frac_b != 0) return 0x7C01;
        else if(exp_b == 0 && frac_b == 0) return 0;
        else { // b -> normal value
            S = S_a ^ S_b;
            exp = exp_a + exp_b - bias;
            frac = frac_a * frac_b;
            if(frac & 0x200000){ // 곱한 수가 자리수를 넘으면 shifting
                exp ++;
                frac = frac >> 1;
            }
            if((frac & 0x03FF) > 0x0200) round ++; // 반올림  
            if(exp>=31){ // 오버플로 발생
                if(S == 0) return 0x7C00;
                else return 0xFC00;
            }
        }
    }
    // frac을 소수점 아래 5자리까지해서 계산했으므로, 10자리만큼 shifting 해줘야 함
    return ((S << 15) & 0x8000) | ((exp << 10) & 0x7c00) | (((frac >> 10) + round) & 0x03FF);
}

char* comparison_function(hpfp a, hpfp b){
    int S=0,exp=0,frac=0;
    int S_a, S_b, exp_a, exp_b, frac_a, frac_b, is_it_pos=1;
    // 비트 연산으로 s, exp, frac 가져오기
    S_a = (a & 0x8000) >> 15;
    S_b = (b & 0x8000) >> 15;
    exp_a = (a & 0x7C00) >> 10;
    exp_b = (b & 0x7C00) >> 10;
    frac_a = ((a & 0x03FF) | 0x0400);
    frac_b = ((b & 0x03FF) | 0x0400);    
    // 각각의 case 별로 분류하여 예외경우 먼저 계산
    if(S_a == 0 && exp_a == 0b11111 && frac_a == 0){
        if(S_b == 0 && exp_b == 0b11111 && frac_b == 0) return "=";
        else if(S_b ==1 && exp_b == 0b11111 && frac_b == 0) return ">";
        else if(exp_b == 0b11111 && frac_b != 0) return "=";
        else return ">";
    }    
    else if(S_a == 1 && exp_a == 0b11111 && frac_a == 0) {
        if(S_b == 0 && exp_b == 0b11111 && frac_b == 0) return "<";
        else if(S_b == 1 && exp_b == 0b11111 && frac_b == 0) return "=";
        else if(exp_b == 0b11111 && frac_b != 0) return "=";
        else return "<";
    }
    else if(exp_a == 0b11111 && frac_a != 0) return "=";
    else {
        if(S_b == 0 && exp_b == 0b11111 && frac_b == 0) return "<";
        else if(S_b == 1 && exp_b == 0b11111 && frac_b == 0) return ">";
        else if(exp_b == 0b11111 && frac_b != 0) return "=";
        else{
            // normal case에서 부호가 다르면 -인게 무조건 더 작음
            if(S_a == 1 && S_b == 0) return "<";
            else if(S_a == 0 && S_b == 1) return ">";
            // 부호가 같은 경우에 대해서 크기 비교
            if(S_a == 0) is_it_pos = 0; // 
            // exp 먼저 비교
            if(exp_a > exp_b){
                if(is_it_pos==0) return ">";
                else return "<";
            }
            else if(exp_a < exp_b) {
                if(is_it_pos==0) return "<";
                else return ">";
            }
            // frac 비교
            if(frac_a > frac_b) {
                if(is_it_pos==0) return ">";
                else return "<";   
            }
            else if(frac_a < frac_b) {
                if(is_it_pos==0) return "<";
                else return ">";
            }
        }
    }
    return "=";

}

char* hpfp_to_bits_converter(hpfp result){
    char* result_arr = (char*)malloc(16*sizeof(char));
    for(int i=15;i>=0;i--){
        result_arr[i] = result % 2 + '0';
        result /= 2;
    }
    return result_arr;
}

char* hpfp_flipper(char* input){
}