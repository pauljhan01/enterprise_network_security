#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct uint128 {
  uint64_t hi;
  uint64_t lo;
} uint128;

void write_arr_to_uint128(uint128 * message, const char plaintext[]);

void modular_power_public(uint128 * output, uint128 * base, uint128 * modulus, uint64_t exp);

void modular_power_private(uint128 * output, uint128 * base, uint128 * modulus, uint128 * exp);

int main() {
	/* Private-Key: (128 bit)                                                                                                                                         */
	/* modulus: */
	/*    00:e0:37:d3:5a:8b:16:0e:b7:f1:19:19:bf:ef:44: */
	/*    09:17 */
	/* publicExponent: 65537 (0x10001) */
	/* privateExponent: */
	/*    00:ca:b1:0c:ca:a4:43:7b:67:11:c9:77:a2:77:fe: */
	/*    00:a1 */
	/* prime1: 18125493163625818823 (0xfb8aafffd4b02ac7) */
	/* prime2: 16442969659062640433 (0xe43129c94cf45f31) */
	/* exponent1: 5189261458857000451 (0x4803f5cd8dcbfe03) */
	/* exponent2: 12850891953204883393 (0xb2578a24fdb3efc1) */
	/* coefficient: 10155582946292377246 (0x8cefe0e210c5a69e) */

	//DO NOT MODIFY
	uint128 modulus = {0xe037d35a8b160eb7LL,  0xf11919bfef440917LL};
	uint128 privateExp = {0x00cab10ccaa4437b67LL,  0x11c977a277fe00a1LL};
	uint64_t pubExp = 65537;
	const char plaintext[] = "Hello !";
	uint128 ciphertext;
	uint128 decrypted;
	//END DO NOT MODIFY

	/* YOUR CODE HERE: Implement RSA encryption, write the encrypted output of plaintext to ciphertext */
	/*
	* We can encrypt using the public key then decrypt using the private key
	* Encryption can occur using these steps
	* 1. Compute public key which consists of the modulus n and the public exponent e. Fortunately, both nunmbers are given to us
	* 2. Compute the integer representation of the message using the PKCS 1.5 padding scheme
	*/
	ciphertext.lo = 0x0;
	ciphertext.hi = 0x0;

	uint128 message;
	message.hi = 0x0; message.lo = 0x0;

	write_arr_to_uint128(&message, plaintext);

	modular_power_public(&ciphertext, &message, &modulus, pubExp);

	//DO NOT MODIFY
	char *encrypted_text = (char*)&ciphertext;
	printf("encrypted=%s\n", encrypted_text);
	//END DO NOT MODIFY

	decrypted.lo = 0x0; decrypted.hi = 0x0;
	/* YOUR CODE HERE: Implement RSA decryption, write the decrypted output of ciphertext to decrypted */
	modular_power_private(&decrypted, &ciphertext, &modulus, &privateExp);

	//DO NOT MODIFY
	char *decrypted_text = (char*)&decrypted;
	printf("decrypted=%s\n", decrypted_text);
	assert(strcmp(plaintext, decrypted_text) == 0);
}

bool compare(uint128 * a, uint128 * b){
	if(a->hi > b->hi){
		return true;
	}
	if(a->hi < b->hi){
		return false;
	}
	return a->lo >= b->lo;
}

void subtract(uint128 * output, uint128 * a, uint128 * b){
	uint128 result;
	result.lo = a->lo - b->lo;
	result.hi = a->hi - b->hi - (a->lo < b->lo);
	*output = result;
}

void left_shift(uint128 * output, uint128 * a){
	output->hi = (a->hi << 1) | (a->lo >> 63);
	output->lo = a->lo << 1;
}

void right_shift(uint128 * output, uint128 * a){
	output->lo = (a->lo >> 1) | (a->hi << 63);
	output->hi = a->hi >> 1;	
}

void add(uint128 * output, const uint128 * a, const uint128 * b){
	uint128 res = {0, 0};
	res.lo = a->lo + b->lo;
	res.hi = a->hi + b->hi + (res.lo < a->lo);
	*output = res;
}

void mul(uint128 * output, const uint128 * a, const uint128 * b){

	unsigned __int128 a_full = ((unsigned __int128)a->hi << 64) | a->lo;
	unsigned __int128 b_full = ((unsigned __int128)b->hi << 64) | b->lo;

	unsigned __int128 res_full = a_full * b_full;

	output->lo = (uint64_t)(res_full & 0xFFFFFFFFFFFFFFFFULL);
	output->hi = (uint64_t)(res_full >> 64);
}

bool is_zero(uint128 *a){
	return a->hi == 0 && a->lo == 0;
}

bool is_odd(uint128 *a){
	return (a->lo & 1) != 0;
}

uint128 add_mod128(uint128 a, uint128 b, uint128 mod) {
    uint128 res;
    uint64_t carry = 0;

    res.lo = a.lo + b.lo;
    if (res.lo < a.lo) carry = 1;
    
    res.hi = a.hi + b.hi + carry;
    
    if ((res.hi < a.hi) || (carry && res.hi == a.hi) || compare(&res, &mod)) {
        subtract(&res, &res, &mod);
    }
    return res;
}

void mul_mod(uint128 * output, uint128 * a, uint128 * b, uint128 * mod){
	uint128 result = {0, 0};
	uint128 temp = *b;

	if(compare(a, mod)){
		a->hi %= mod->hi ? mod->hi : 0xFFFFFFFFFFFFFFFFULL;
	}

	while(!is_zero(b)){
		if(is_odd(b)){
			// add(&result, &result, a);
			result = add_mod128(result, *a, *mod);
		}

		*a = add_mod128(*a, *a, *mod);

		right_shift(b, b);
	}

	*output = result;
	*b = temp;
}

void modular_power_public(uint128 * output, uint128 * base, uint128 * modulus, uint64_t exp){
	uint128 result = {0, 1};

	if(compare(base, modulus)){
		base->lo = base->lo % modulus->lo;
	}

	while(exp > 0){

		if(exp % 2 == 1){
			mul_mod(&result, &result, base, modulus);
		}
		
		uint128 temp;
		uint128 temp1 = *base;
		mul_mod(&temp, base, &temp1, modulus);
		*base = temp;

		exp = exp >> 1;
	}

	*output = result;
}



void modular_power_private(uint128 * output, uint128 * base, uint128 * modulus, uint128 * exp){
	uint128 result = {0, 1};

	if(compare(base, modulus)){
		base->lo = base->lo % modulus->lo;
	}

	while(!is_zero(exp)){

		if(is_odd(exp)){
			mul_mod(&result, &result, base, modulus);
		}
		
		uint128 temp;
		uint128 temp1 = *base;
		mul_mod(&temp, base, &temp1, modulus);
		*base = temp;

		right_shift(exp, exp);
	}

	*output = result;
}

void write_arr_to_uint128(uint128 * message, const char plaintext[]){
	int len = strlen(plaintext) + 1;
	char * encrypt = (char *)message;
	for(int i = 0; i < len; i++){
		encrypt[i] = plaintext[i];
	}

}

