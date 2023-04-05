#include "hpfp.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define MAX_HPFP 65519

int power(int a, int b) {
	int result = 1;
	for (; b >= 1; b--) {
		result *= a;
	}
	return result;
}

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

	if (input > MAX_HPFP) {
		// POS INFINITY
		// 0 11111 0000000000
		result_hpfp[0] = 0;
		for (i = 1; i <= 5; i++)
			result_hpfp[i] = 1;
		for (i = 6; i <= 15; i++)
			result_hpfp[i] = 0;
	}
	else if (input < -MAX_HPFP) {
		// NEG INFINITY
		// 1 11111 0000000000
		result_hpfp[0] = 1;
		for (i = 1; i <= 5; i++)
			result_hpfp[i] = 1;
		for (i = 6; i <= 15; i++)
			result_hpfp[i] = 0;
	}
	else if (input == 0) {
		// ZERO
		// 0 00000 0000000000
		result_hpfp[0] = 0;
		for (i = 1; i <= 5; i++)
			result_hpfp[i] = 0;
		for (i = 6; i <= 15; i++)
			result_hpfp[i] = 0;
	}
	else {
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
	int check_inf = 1;
	int check_nan = 1;

	// 1.		2진법으로 변환
	for (i = 15; i >= 0; i--) {
		input_hpfp[i] = input % 2;
		input /= 2;
	}

	// infinity 확인
	// [1] ~ [5] : 11111 ?
	for (i = 1; i <= 5; i++) {
		if (input_hpfp[i] == 0) {
			check_inf = 0;
			check_nan = 0;
			break;
		}
	}
	if (check_inf) {
		// [6] ~ [10] : 0000000000?
		for (i = 6; i <= 15; i++) {
			if (input_hpfp[i] == 1) {
				break;
			}
		}
		check_nan = 0;
	}
	// exp = 11111, frac = 00...0 일때
	if (check_inf) {
		if (input_hpfp[0] == 0)		result_int = INT_MAX;
		else if (input_hpfp[0] == 1)	result_int = INT_MIN;
	}
	// exp = 11111, frac != 00...0 일때
	else if (check_nan) {
		result_int = INT_MIN;
	}
	else {
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
	}

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

	if (input > MAX_HPFP) {
		// POS INFINITY
		// 0 11111 0000000000
		result_hpfp[0] = 0;
		for (i = 1; i <= 5; i++)
			result_hpfp[i] = 1;
		for (i = 6; i <= 15; i++)
			result_hpfp[i] = 0;
	}
	else if (input < -MAX_HPFP) {
		// NEG INFINITY
		// 1 11111 0000000000
		result_hpfp[0] = 1;
		for (i = 1; i <= 5; i++)
			result_hpfp[i] = 1;
		for (i = 6; i <= 15; i++)
			result_hpfp[i] = 0;
	}
	else if (input == 0) {
		// ZERO
		// 0 00000 0000000000
		result_hpfp[0] = 0;
		for (i = 1; i <= 5; i++)
			result_hpfp[i] = 0;
		for (i = 6; i <= 15; i++)
			result_hpfp[i] = 0;
	}
	else {
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
	}
	
	// 4-4.		완성된 배열을 unsigned short로 파싱
	for (i = 15, tmp = 1; i >= 0; i--) {
		result += result_hpfp[i] * tmp;
		tmp *= 2;
	}

	return result;
}

float hpfp_to_float_converter(hpfp input){
	short int input_hpfp[16];        	  // input hpfp
	float result_float = 1;				  // output float
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
	for (i = 6, frac_tmp = 0.5; i <= 15; i++) {
		result_float += input_hpfp[i] * frac_tmp;
		frac_tmp /= 2;
	}
	if (E >= 0)
		result_float *= power(2, E);
	else
		result_float /= power(2, -E);
	// 2-4. 	부호 포함
	if (S == 1)	result_float = -result_float;

	return result_float;
}

hpfp addition_function(hpfp a, hpfp b){
	
}

hpfp multiply_function(hpfp a, hpfp b){
	
}

char* comparison_function(hpfp a, hpfp b){
	short int input_hpfp_1[16];         	  // input hpfp
	short int input_hpfp_2[16];         	  // input hpfp
	hpfp result_hpfp = 0;					  // output int
	int i;
	int inputs_are_pos = 1;

	// 1.		2진법으로 변환
	for (i = 15; i >= 0; i--) {
		input_hpfp_1[i] = a % 2;
		input_hpfp_2[i] = b % 2;
		a /= 2;	b /= 2;
	}

	// 2.		대소 비교
	// 2-1.		부호 비교
	if (input_hpfp_1[0] < input_hpfp_2[0])		return ">";
	else if (input_hpfp_1[0] > input_hpfp_2[0]) return "<";
	// 2-2.		두 input의 음수 양수 구분
	if (input_hpfp_1[0] == 1)
		inputs_are_pos = 0;
	// 2-3.		bit 별로 비교하며, 더 클 경우 부등호 return
	for (i = 1; i <= 15; i++) {
		if (input_hpfp_1[i] > input_hpfp_2[i]) {
			if (inputs_are_pos)
				return ">";
			else
				return "<";
		}
		else if (input_hpfp_1[i] < input_hpfp_2[i]) {
			if (inputs_are_pos)
				return "<";
			else
				return ">";
		}
	}
	// 2-4.		끝까지 같다면, 같은 수이므로 등호 return
	return "=";
}

float float_flipper(float input){
	short int int_deci_to_binary[11];     // input int의 2진법 저장 (정수부)
	short int frac_deci_to_binary[5];	  // input int의 2진법 저장 (소수부)
    short int result_hpfp[16];        	  // output hpfp의 저장
    hpfp result = 0;				  	  // 최종 output hpfp
    int cnt = -1;               	  	  // 2진법 숫자 개수 (E를 구하기 위함)
    int S, M, E;					  	  // S, M, E
	int exp = 0;
	int frac = 0;
	int tmp, i, bias;
	int input_int;
	int input_was_negative = 0;			  // input 부호 기억
	float input_frac, frac_tmp;
	float result_float = 1;

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
	
	// 5. 		flip
	for (i = 0; i <= 15; i++) {
		if (result_hpfp[i] == 1)
			result_hpfp[i] = 0;
		else
			result_hpfp[i] = 1;
	}

	// 6.		다시 float로 파싱
	// 6-1.		[0] = S
	S = result_hpfp[0];
	// 6-2.		[1:5] = exp
	for (i = 5, tmp = 1; i >= 1; i--) {
		exp += result_hpfp[i] * tmp;
		tmp *= 2;
	}
	E = exp - 15;	// E = exp - bias
	// 6-3.		[6:15] = frac
	// 구한 E를 통해, 자릿수에 맞게 2진법 계산
	for (i = 6, frac_tmp = 0.5; i <= 15; i++) {
		result_float += result_hpfp[i] * frac_tmp;
		frac_tmp /= 2;
	}
	if (E >= 0)
		result_float *= power(2, E);
	else
		result_float /= power(2, -E);
	// 6-4. 	부호 포함
	if (S == 1)	result_float = -result_float;

	return result_float;
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
