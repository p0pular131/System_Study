#include "hpfp.h"
#include <limits.h>
#include <stdlib.h>
#define exp_size 5
#define frac_size 10
#define bias 15
#define max_hp 65504

int* uint_to_Binary(int input){ // input을 unsigned Binary로 변환
    if(input<0) input = -input;
    int size = 0, temp = input;
    while(temp>0) {
        temp /= 2;
        size += 1;
    }
    size += 1;
    int* arr = (int*)malloc(size*sizeof(int));
 
    arr[size-1] = -1;
    int idx = 1;
    while(input > 0){
        arr[size-idx-1] = input%2;
        input /= 2;
        idx += 1; 
    }
    return arr;
}

int* float_to_Binary(float input){
    int* arr = (int*)malloc(10*sizeof(int));
    if(input<0) input = -input;
    float temp=input;
    for(int i=0;i<10;i++){
        temp *= 2;
        arr[i] = (int)temp;
        if(temp>=1) temp -= 1; 
    }
    return arr;
}

hpfp Binary_to_Decimal(int* arr){
    hpfp num = 0;
    int size = 0, curr = 1;
    while(arr[size]!=-1) size += 1;
    for(int i=size-1;i>=0;i--){
        num += curr*arr[i];
        curr *= 2;
    }
    return num;
}

float Binary_to_float(int* arr){ // 여기서 반올림때문에 문제 생기는 듯. 
    float num = 0, curr = 0.5;
    int size=0;
    while(arr[size]!=-1) size += 1;
    for(int i=0;i<size;i++){
        num += curr*arr[i];
        curr *= 0.5;
    }
    return num;
}

hpfp int_converter(int input){
    int arr[17];
    arr[16] = -1;
    if(input==0){ // +0으로 저장
        for(int i=0;i<16;i++) arr[i] = 0;
        return Binary_to_Decimal(arr);
    }
    else if(input>65504) { // +inf 저장
        arr[0] = 0;
        for(int i=1;i<6;i++) arr[i] = 1;
        for(int i=6;i<16;i++) arr[i] = 0;
        return Binary_to_Decimal(arr);
    } 
    else if(input<-655504) { // -inf 저장
        arr[0] = 1;
        for(int i=1;i<6;i++) arr[i] = 1;
        for(int i=6;i<16;i++) arr[i] = 0;
        return Binary_to_Decimal(arr);
    }
    // 이외의 경우 
    // sign-bit 먼저 계산
    if(input>0) arr[0] = 0;
    else if(input<0) arr[0] = 1;
    //주어진 수 2진수로 변환
    int* bin = uint_to_Binary(input);
    int size = 0;
    int frac[10];
    while(bin[size]!=-1) size += 1; // 이진수 사이즈 계산
    for(int i=1;i<size;i++){
        frac[i-1] = bin[i];
    }
    for(int i=size-1;i<frac_size;i++){
        frac[i] = 0;
    }
    int E = size-1;
    int exp_deci = E + bias;
    int* exp_arr = uint_to_Binary(exp_deci);
    int size_e = 0, zero_cnt = 0;
    while(exp_arr[size_e]!=-1) size_e+=1;
    for(int i=0;i<5-size_e;i++){
        arr[i+1] = 0;
        zero_cnt += 1;
    }
    for(int i=1+zero_cnt;i<6;i++){
        arr[i] = exp_arr[i-1-zero_cnt];
    }
    for(int i=6;i<16;i++){
        arr[i] = frac[i-6];
    }
    return Binary_to_Decimal(arr);      
}

int hpfp_to_int_converter(hpfp input){
    char* arr = hpfp_to_bits_converter(input);
    int exp[exp_size+1], frac[frac_size+1], cnt_1 = 0, cnt_0 = 0;
    exp[exp_size] = -1;
    frac[frac_size] = -1;
    for(int i=0;i<exp_size;i++){ // exp 값 저장
        if(arr[i+1]=='1'){
            exp[i] = 1;
            cnt_1 += 1;
        } 
        else {
            exp[i] = 0;
            cnt_0 += 1;
        }
    }
    // frac 배열 생성
    int frac_zero = 1;
    for(int i=0;i<frac_size;i++){
        if(arr[i+exp_size+1]=='1'){
            frac[i] = 1;
            frac_zero = 0;
        }
        else frac[i] = 0;
    }
    int E = 0;
    float M = 0;
    if(cnt_1==5){ // +inf,inf or NaN frac이 다 0이면 전자, 아니면 후자
        if(frac_zero==1){ // inf 
            if(arr[0]=='1') return -2.147483648e9;
            else return 2.147483647e9;
        }
        else return -2.147483648e9;
    }
    if(cnt_0==5){ // frac+1, E값 계산 다르게
        E = 1-bias;
        M = Binary_to_float(frac);
    }
    else { //
        E = (int)Binary_to_Decimal(exp)-bias;
        M = Binary_to_float(frac)+1;
    }
    int result = 1;
    if(arr[0]=='1') result = -1;
    if(E>0){
        for(int i=0;i<E;i++) result *= 2;
    }
    else if(E<0){
        for(int i=0;i<E;i++) result /= 2;
    }
    double result_num = result*M;
    return (int)result_num;
}

hpfp float_converter(float input){
    int arr[17];
    arr[16] = -1;
    if(input==0){ // +0으로 저장
        for(int i=0;i<16;i++) arr[i] = 0;
        return Binary_to_Decimal(arr);
    }
    else if(input>65504) { // +inf 저장
        arr[0] = 0;
        for(int i=1;i<6;i++) arr[i] = 1;
        for(int i=6;i<16;i++) arr[i] = 0;
        return Binary_to_Decimal(arr);
    } 
    else if(input<-655504) { // -inf 저장
        arr[0] = 1;
        for(int i=1;i<6;i++) arr[i] = 1;
        for(int i=6;i<16;i++) arr[i] = 0;
        return Binary_to_Decimal(arr);
    }
    // 이외의 경우 
    // sign-bit 먼저 계산
    if(input>0) arr[0] = 0;
    else if(input<0) arr[0] = 1;
    int frac[10];
    // 정수부, 소수부 분리
    int input_int = (int)input;
    float input_float = input-input_int;
    // 각각 2진수로 변환
    int* arr_int = uint_to_Binary(input_int);
    int* arr_float = float_to_Binary(input_float);
    int size = 0; // 정수부가 차지하는 크기를 알아야 함.
    while(arr_int[size]!=-1) size += 1;
    int E;
    // 정수부가 존재하는 경우 -> 나머지만 frac을 채움 round to zero
    if(size>1 || (size==1 && arr_int[0]==1)){ 
        for(int i=1;i<size;i++) frac[i-1] = arr_int[i];
        for(int i=size;i<frac_size+1;i++) frac[i-1] = arr_float[i-size];
        E = size - 1;
    }
    else{ // 정수부가 없는 경우 -> 전부 소수부로 채움
        E = 0;
        for(int i=0;i<frac_size;i++) frac[i] = arr_float[i];
    }
    int exp = E + bias;
    int* exp_arr = uint_to_Binary(exp);
    int size_e = 0, zero_cnt = 0;
    while(exp_arr[size_e]!=-1) size_e+=1;
    for(int i=0;i<5-size_e;i++){
        arr[i+1] = 0;
        zero_cnt += 1;
    }
    for(int i=1+zero_cnt;i<6;i++){
        arr[i] = exp_arr[i-1-zero_cnt];
    }
    for(int i=6;i<16;i++){
        arr[i] = frac[i-6];
    }
    return Binary_to_Decimal(arr);      
}

float hpfp_to_float_converter(hpfp input){
    char* arr = hpfp_to_bits_converter(input);
    int exp[exp_size+1], frac[frac_size+1], cnt_1 = 0, cnt_0 = 0;
    exp[exp_size] = -1;
    frac[frac_size] = -1;
    for(int i=0;i<exp_size;i++){ // exp 값 저장
        if(arr[i+1]=='1'){
            exp[i] = 1;
            cnt_1 += 1;
        } 
        else {
            exp[i] = 0;
            cnt_0 += 1;
        }
    }
    // frac 배열 생성
    int frac_zero = 1;
    for(int i=0;i<frac_size;i++){
        if(arr[i+exp_size+1]=='1'){
            frac[i] = 1;
            frac_zero = 0;
        }
        else frac[i] = 0;
    }
    int E = 0;
    float M = 0;
    // 직접 float 주소 할당하여 inf, NaN 표현
    float pos_inf,neg_inf,pos_NaN,neg_NaN; 
    *((unsigned int*)&pos_inf) = 0x7F800000;
    *((unsigned int*)&neg_inf) = 0xFF800000;
    *((unsigned int*)&pos_NaN) = 0x7F810000;
    *((unsigned int*)&neg_NaN) = 0x7F810000;
    if(cnt_1==5){ 
        if(frac_zero==1){ 
            if(arr[0]=='1') return  neg_inf;
            else return pos_inf;
        }
        else {
            if(arr[0]=='1') return neg_NaN;
            else return pos_NaN;
        }
    }
    if(cnt_0==5){ // frac+1, E값 계산 다르게
        E = 1-bias;
        M = Binary_to_float(frac);
    }
    else { //
        E = (int)Binary_to_Decimal(exp)-bias;
        M = Binary_to_float(frac)+1;
    }
    float result = 1;
    if(E>0){
        for(int i=0;i<E;i++) result *= 2;
    }
    else if(E<0){
        for(int i=0;i<-E;i++) result /= 2;
    }
    float result_num = result*M;
    if(arr[0]=='1') result_num *= -1;
    return result_num;    
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
    hpfp input_hp = 0;
    char is_it_pos = input[0];
    char* result_arr = (char*)malloc(sizeof(char) * 16);
    for(int i=15, curr = 1;i>=0;i--){
        input_hp += (input[i] - '0')*curr;
        curr *= 2;
    }
    float hp = hpfp_to_float_converter(input_hp);
    double hp_num = hp;
    if(input[0]=='1') hp_num *= -1;
    int cnt = 0; // 소수점 아래 자리수 계산
    while(hp_num != (int)hp_num){
        hp_num *= 10;
        cnt ++;
    }
    int temp = (int)hp_num, res, n=0, length = 0;
    while(temp != 0){
        res = temp % 10;
        n = n*10 + res;
        temp /= 10;
        length ++;
    }
    double flipped = (double)n;
    if(cnt != 0) cnt = length - cnt;
    while(cnt!=0){
        flipped /= 10;
        cnt --;
    }
    if(is_it_pos=='1') flipped *= -1;
    float ans_flipped = (float)flipped;
    hpfp flped_result = float_converter(ans_flipped);
    result_arr = hpfp_to_bits_converter(flped_result);
    return result_arr;
}
