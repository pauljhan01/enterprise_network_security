#include <assert.h>
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

typedef struct message_text{
	char * string;
	uint32_t len;
}msg_txt_t;

void write_arr_to_uint128(uint128 * message, const char plaintext[]);

void modular_power_public(uint128 * ciphertext, uint128 * message, uint128 * modulus, uint64_t exp);

void modular_power_private(uint128 * ciphertext, uint128 * message, uint128 * modulus, uint64_t exp);

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
	message.lo = 0x0;
	message.hi = 0x0;
	write_arr_to_uint128(&message, plaintext);

	message.lo = 4;
	message.hi = 0;

	modulus.lo = 497;
	modulus.hi = 0;

	pubExp = 13;

	modular_power_public(&ciphertext, &message, &modulus, pubExp);

	printf("%lu\n", ciphertext.lo);	
	//DO NOT MODIFY
	char *encrypted_text = (char*)&ciphertext;
	printf("encrypted=%s\n", encrypted_text);
	//END DO NOT MODIFY

	/* YOUR CODE HERE: Implement RSA decryption, write the decrypted output of ciphertext to decrypted */

	//DO NOT MODIFY
	char *decrypted_text = (char*)&decrypted;
	printf("decrypted=%s\n", decrypted_text);
	assert(strcmp(plaintext, decrypted_text) == 0);
}

int compare(uint128 * a, uint128 * b){
	if(a->hi != b->hi){
		return (a->hi > b->hi) ? 1 : -1;
	}

	if(a->lo != b->lo){
		return (a->lo > b->lo) ? 1 : -1;
	}

	return 0;
}

void subtract(uint128 * output, uint128 * a, uint128 * b){
	output->lo = a->lo - b->lo;
	output->hi = a->hi - b->hi;

	if (a->lo < b->lo){
		output->hi--;
	}
}

void left_shift(uint128 * output, uint128 * a){
	output->hi = (a->hi << 1) | (a->lo >> 63);
	output->lo = a->lo << 1;
}

uint64_t uint128_to_64_mod(uint128 * dividend, uint64_t divisor){
	if(divisor == 0){
		return 0;
	}

	if(dividend->hi == 0){
		return dividend->lo % divisor;
	}

	uint64_t remainder = 0;
	remainder = dividend->hi % divisor;

	for(int i = 63; i >= 0; i--){
		remainder = (remainder << 1) | ((dividend->lo >> i) & 1);
		if(remainder >= divisor){
			remainder -= divisor;
		}
	}

	return remainder;
}

void uint128_to_uint128_mod(uint128 * output, uint128 * dividend, uint128 * divisor){
	if (divisor->hi == 0 && divisor->lo == 0){
		output->hi = 0;
		output->lo = 0;
		return;
	}

	if(compare(dividend, divisor) < 0){
		*output = *dividend;
		return;
	}

	uint128 remainder = *dividend;

	int shift = 0;
	uint128 shifted_divisor;
	shifted_divisor.hi = divisor->hi;
	shifted_divisor.lo = divisor->lo;
	uint128 temp;

	while(1){
		left_shift(&temp, &shifted_divisor);
		if(compare(&temp, dividend) > 0){
			break;
		}

		shifted_divisor = temp;
		shift++;
	}

	for(int i = shift; i >= 0; i++){
		if(compare(&remainder, &shifted_divisor) >= 0){
			uint128 remainder_temp;
			subtract(&remainder, &remainder, &shifted_divisor);	
		}

		shifted_divisor.lo = (shifted_divisor.lo >> 1) | (shifted_divisor.hi << 63);
		shifted_divisor.hi = (shifted_divisor.hi >> 1);
	}

	output->hi = remainder.hi;
	output->lo = remainder.lo;	
}

void add(uint128 * output, const uint128 * a, const uint128 * b){
	output->lo = a->lo + b->lo;
	output->hi = a->hi + b->hi;

	if(output->lo < a->lo){
		output->hi++;
	}
}

void uint64_to_uint128_mul(uint128 * output, uint64_t a, uint64_t b){
	uint64_t a_lo = a & 0xFFFFFFFFULL;
    uint64_t a_hi = a >> 32;
    uint64_t b_lo = b & 0xFFFFFFFFULL;
    uint64_t b_hi = b >> 32;
    
    uint64_t p0 = a_lo * b_lo;
    uint64_t p1 = a_lo * b_hi;
    uint64_t p2 = a_hi * b_lo;
    uint64_t p3 = a_hi * b_hi;
    
    uint64_t middle = p1 + (p0 >> 32);
    middle += p2;
    
    output->lo = (middle << 32) | (p0 & 0xFFFFFFFFULL);
    output->hi = p3 + (middle >> 32);
    
    if (middle < p1) {
        output->hi += 0x100000000ULL;
    }	
}

void mul(uint128 * output, const uint128 * a, const uint128 * b){
	uint128 low_product;
	uint64_to_uint128_mul(&low_product, a->lo, b->lo);

	uint64_t mid1 = a->hi * b->lo;
	uint64_t mid2 = a->lo * b->hi;

	output->lo = low_product.lo;
	output->hi = low_product.hi + mid1 + mid2;
}

void modular_power_public(uint128 * ciphertext, uint128 * message, uint128 * modulus, uint64_t exp){
		if((modulus->hi + modulus->lo) == 1){
			ciphertext->hi = 0;
			ciphertext->lo = 0;
			return;
		}

		ciphertext->lo = 1;
		ciphertext->hi = 0;

		uint128 temp;

		uint128 base = *message;
		uint128_to_uint128_mod(&base, &base, modulus);

		while (exp > 0){
			if(exp % 2 == 1){
				temp = *ciphertext;
				mul(ciphertext, &temp, &base);

				temp = *ciphertext;
				uint128_to_uint128_mod(ciphertext, &temp, modulus);
			}
			exp = exp >> 1;

			mul(&base, &base, &base);
			uint128_to_uint128_mod(&base, &base, modulus);
		}
}

void write_arr_to_uint128(uint128 * message, const char plaintext[]){
	int len = strlen(plaintext) + 1;

	for(int i = 0; i < len/2; i++){
		message->lo |= plaintext[i] << (i * 8);
	}

	uint32_t shift = 0;
	for(int i = len/2; i < len; i++){
		message->hi |= plaintext[i] << (shift * 8);
		shift++;
	}

}

