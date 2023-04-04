#include "hpfp.h"
#include <stdlib.h>
#include <stdio.h>

hpfp int_converter(int input){
    short int deci_to_binary[11];     // input int의 2진법 저장
    short int result_hpfp[16];        // output hpfp의 저장
    hpfp result = 0;				  // 최종 output hpfp
    int cnt = -1;               	  // 2진법 숫자 개수 (E를 구하기 위함)
    int S, M, E;					  // S, M, E
    int bias, exp;					  // exp, bias
	int tmp, i;

	// 0.		array 초기화
	for (i = 0; i < 11; i++)
		deci_to_binary[i] = 0;
	for (i = 0; i < 16; i++)
		result_hpfp[i] = 0;

	// 1. 		input을 2진법으로 변환
	if (input < 0)	input = -input;
    for (i = 0; input >= 1; i++) {
        deci_to_binary[i] = input % 2;
        input /= 2;
        cnt++;
    }

    // 2. 		(-1)^S * M * 2^E 형태로 나타내기
    // 2-1. 	S = 0 (양수 or 0) ; 1 (음수)
    if (input < 0) S = 1;
    else S = 0;
    // 2-2. 	M = deci_to_binary[0:cnt-1]
    // 2-3.		E = cnt;
    E = cnt;

	// 3. 		exp, frac으로 변환
    bias = 15;		// bias = 2^(5-1)-1 = 15
    exp = bias + E;	// exp = bias + E
	// frac의 경우, 2진법 변환한 수에서 맨 앞 1 제외
	
    // 4. 결과 hpfp에 채우기
    // 4-1. 	s 	: [0]
    result_hpfp[0] = S;
    // 4-2.		exp : [1:5]
    // exp를 2진법으로 변환하며, 역순으로 대입
    for (i = 5; i >= 1; i--) {
        result_hpfp[i] = exp % 2;
        exp /= 2;
    }
    // 4-3.		frac : [6:15]
    // M에서 cnt 인덱스 제외하고, 역순으로 대입
    for (i = 6; i < cnt+6; i++) {
        // [cnt-1] ~ 0 -> [6] ~ [6+cnt-1]
        result_hpfp[i] = deci_to_binary[cnt-i+5];
    }
	// 4-4.		완성된 배열을 unsigned short로 파싱
	for (i = 15, tmp = 1; i >= 0; i--) {
		result += result_hpfp[i] * tmp;
		tmp *= 2;
	}

	return result;
}

int hpfp_to_int_converter(hpfp input){
	short int input_hpfp[16];         // input hpfp
	int result_int = 0;				  // output int
	int S, M, E;
	int exp = 0;
	int frac = 0;
	int tmp, i;

	// 1.		2진법으로 변환
	for (i = 15; i >= 0; i--) {
		input_hpfp[i] = input % 2;
		input /= 2;
	}

	// 2.		값 파싱
	// 2-1.		[0] = S
	S = input_hpfp[0];
	// 2-2.		[1:5] = exp
	for (i = 5, tmp = 1; i >= 1; i--) {
		exp += input_hpfp[i] * tmp;
		tmp *= 2;
	}
	E = exp - 15;	// E = exp - bias
	// 2-3.		[6:15] = frac
	// 구한 E를 통해, 자릿수에 맞게 2진법 계산
	for (i = E+5, tmp = 1; i >= 6; i--) {
		result_int += input_hpfp[i] * tmp;
		tmp *= 2;
	}
	// 맨 앞자리 포함해야 함 (M = 1 + frac)
	result_int += tmp;
	if (S == -1)	result_int = -result_int;

	return result_int;
}

hpfp float_converter(float input){
	short int int_deci_to_binary[11];     // input int의 2진법 저장 (정수부)
	short int frac_deci_to_binary[5];	  // input int의 2진법 저장 (소수부)
    short int result_hpfp[16];        	  // output hpfp의 저장
    hpfp result = 0;				  	  // 최종 output hpfp
    int cnt = -1;               	  	  // 2진법 숫자 개수 (E를 구하기 위함)
    int S, M, E;					  	  // S, M, E
    int bias, exp;					  	  // exp, bias
	int tmp, i;
	int input_int;
	int input_was_negative = 0;			  // input 부호 기억
	float input_frac, frac_tmp;

	// 0.		array 초기화
	for (i = 0; i < 11; i++)
		int_deci_to_binary[i] = 0;
	for (i = 0; i < 5; i++)
		frac_deci_to_binary[i] = 0;
	for (i = 0; i < 16; i++)
		result_hpfp[i] = 0;

	// 1. 		정수부, 소수부로 분리 후 2진법으로 변환
	if (input < 0) {
		input_was_negative = 1;
		input = -input;
	}
	input_int = (int)input;
	input_frac = input - input_int;
	
	// 정수부
    for (i = 0; input_int >= 1; i++) {
        int_deci_to_binary[i] = input_int % 2;
        input_int /= 2;
        cnt++;
    }
	// 소수부
	frac_tmp = input_frac;
	for (i = 0; i < 5; i++) {
		frac_tmp *= 2;
		frac_deci_to_binary[i] = (int)frac_tmp;
		frac_tmp -= (int)frac_tmp;
	}

    // 2. 		(-1)^S * M * 2^E 형태로 나타내기
    // 2-1. 	S = 0 (양수 or 0) ; 1 (음수)
	if (input_was_negative) S = 1;
    else S = 0;
    // 2-2. 	M = int_deci_to_binary[0:cnt-1] 그리고 frac_deci_to_binary[0:4] 이어붙이기
    // 2-3.		E = cnt;
    E = cnt;

	// 3. 		exp, frac으로 변환
    bias = 15;		// bias = 2^(5-1)-1 = 15
    exp = bias + E;	// exp = bias + E
	
    // 4. 결과 hpfp에 채우기
    // 4-1. 	s 	: [0]
	result_hpfp[0] = S;
    // 4-2.		exp : [1:5]
    // exp를 2진법으로 변환하며, 역순으로 대입
	for (i = 5; i >= 1; i--) {
        result_hpfp[i] = exp % 2;
        exp /= 2;
    }
    // 4-3.		frac : [6:15]
	for (i = 6; i < cnt+6; i++) {
        // [cnt-1] ~ 0 -> [6] ~ [6+cnt-1]
        result_hpfp[i] = int_deci_to_binary[cnt-i+5];
    }
	for (i = cnt+6; i < cnt+11; i++) {
		result_hpfp[i] = frac_deci_to_binary[i-cnt-6];
	}
	for (i=0; i<16; i++)
		printf("%d", result_hpfp[i]);
	// 4-4.		완성된 배열을 unsigned short로 파싱
	for (i = 15, tmp = 1; i >= 0; i--) {
		result += result_hpfp[i] * tmp;
		tmp *= 2;
	}

	return result;
}

float hpfp_to_float_converter(hpfp input){
	short int input_hpfp[16];        	  // input hpfp
	float result_float = 0;				  // output float
	int S, M, E;
	int exp = 0;
	int frac = 0;
	int tmp, i;
	float frac_tmp;

	// 1.		2진법으로 변환
	for (i = 15; i >= 0; i--) {
		input_hpfp[i] = input % 2;
		input /= 2;
	}

	// 2.		값 파싱
	// 2-1.		[0] = S
	S = input_hpfp[0];
	// 2-2.		[1:5] = exp
	for (i = 5, tmp = 1; i >= 1; i--) {
		exp += input_hpfp[i] * tmp;
		tmp *= 2;
	}
	E = exp - 15;	// E = exp - bias
	// 2-3.		[6:15] = frac
	// 구한 E를 통해, 자릿수에 맞게 2진법 계산
	// 정수부
	for (i = E+5, tmp = 1; i >= 6; i--) {
		result_float += input_hpfp[i] * tmp;
		tmp *= 2;
	}
	result_float += tmp;
	// 소수부
	for (i = E+6, frac_tmp = 1; i < 16; i++) {
		frac_tmp /= 2;
		result_float += input_hpfp[i] * frac_tmp;
	}
	// 맨 앞자리 포함해야 함 (M = 1 + frac)
	if (S == 1)	result_float = -result_float;

	return result_float;
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
	int i;

	// 1. 		return 하기 위한 문자열 선언
	char* result_str = (char*)malloc(sizeof(char) * 16);

	// 2. 		2로 나누면서 역순으로 문자 저장
	for (i = 15; i >= 0; i--) {
		result_str[i] = result % 2 + '0';
		result /= 2;
	}

	return result_str;
}
