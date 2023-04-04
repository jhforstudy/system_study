#include "hpfp.h"
#include <stdio.h>
#include <stdlib.h>

hpfp int_converter(int input){
    int deci_to_binary[11];     // 변환한 2진법 저장
    int result_hpfp[16];        // 결과
    int i = 0;
    int cnt = -1;               // 2진법의 마지막 index
    int S, M, E;
    int bias, exp;
    int tmp;

    printf("%d\n", input);

    // array 초기화
    for (i=0; i<11; i++)
        deci_to_binary[i] = 0;

	// input을 2진법으로 변환
    for (i=0; input>=1; i++) {
        deci_to_binary[i] = input % 2;
        printf("%d ", deci_to_binary[i]);
        input /= 2;
        cnt++;
    }
    // (-1)^S * M * 2^E 형태로 나타내기
    // S = 0 (양수 or 0) ; 1 (음수)
    if (input < 0) S = 1;
    else S = 0;
    // M = deci_to_binary[0:cnt-1]
    // E = cnt;
    E = cnt;

    // bias = 2^(5-1)-1 = 15
    // exp = bias + E
    bias = 15;
    exp = bias + E;

    // 결과 hpfp에 채우기
    // S    : [0]
    result_hpfp[0] = S;
    // exp  : [1:5]
    // exp를 2진법으로 변환하며, 역순으로 대입
    for (i=5; i>=1; i++) {
        result_hpfp[i] = exp % 2;
        exp /= 2;
    }
    // frac : [6:15]
    // M에서 cnt-1 인덱스 제외하고, 역순으로 대입
    for (i=0; i<cnt; i++) {
        // [6] ~ [6+cnt-1]
        result_hpfp[i+6] = deci_to_binary[cnt-1-i];
    }
    for (i=cnt; i<10; i++) {
        result_hpfp[i+6] = 0;
    }

    for (i=0; i<16; i++)
        printf("%d", result_hpfp[i]);
}

int hpfp_to_int_converter(hpfp input){
}

hpfp float_converter(float input){
}

float hpfp_to_float_converter(hpfp input){
}

hpfp addition_function(hpfp a, hpfp b){
	
}

hpfp multiply_function(hpfp a, hpfp b){
	
}

char* comparison_function(hpfp a, hpfp b){

}

float float_flipper(float input){

}

char* hpfp_to_bits_converter(hpfp result){

}
