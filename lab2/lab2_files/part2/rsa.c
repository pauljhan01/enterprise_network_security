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

void pad_pkcs15(char * padded_plaintext, const char plaintext[]);

void pack_char_arr_uint128(msg_txt_t * message_txt, uint128 * message);


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
	#define NUM_PADDED_BYTES 8
	int len = NUM_PADDED_BYTES + strlen(plaintext) + 1;
	
	msg_txt_t * message_txt = (msg_txt_t *)calloc(1, sizeof(msg_txt_t));
	message_txt->string = (char *)calloc(len, sizeof(char));
	message_txt->len = len;
	pad_pkcs15(message_txt->string, plaintext);
	
	uint128 * message = (uint128 *)calloc(1, sizeof(uint128));
	pack_char_arr_uint128(message_txt, message);	

	free(message_txt->string);
	free(message_txt);
	
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

void pack_char_arr_uint128(msg_txt_t * message_txt, uint128 * message){
	uint32_t shifts = 0;
	for(int i = 0; i < message_txt->len; i++){
		if(i * 8 >= 64){
			message->hi = message->hi | (message_txt->string[i] << (i*8));
		}else{
			message->lo = message->lo | (message_txt->string[i] << (i*8));
		}
		printf("%x", message_txt->string[i]);
	}
	printf("\n");
}

void pad_pkcs15(char * padded_plaintext, const char plaintext[]){
	int len = strlen(plaintext);
	padded_plaintext[0] = 0x00;
	padded_plaintext[1] = 0x02;

	for(int i = 2; i < NUM_PADDED_BYTES - 1; i++){
		padded_plaintext[i] = 0x03;
	}

	padded_plaintext[NUM_PADDED_BYTES - 1] = 0x00;

	//include NULL terminator
	for(int i = 0; i < len + 1; i++){
		padded_plaintext[i + NUM_PADDED_BYTES] = plaintext[i];
	}
	
	
}
