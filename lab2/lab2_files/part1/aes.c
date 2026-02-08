#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void encrypt_aes_ctr(char * plaintext, char * s_box, char * iv, char * key, char * enc_buf);

void decrypt_aes_ctr(char * encrypted_text, char * decrypted_text, char * s_box, char * iv, char * key);

int main() {
	//DO NOT MODIFY

	unsigned char s[256] = 
	{
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
	};


	unsigned char enc_buf[128];
	unsigned char plaintext[1][32] = {
	{0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51}
	};
	unsigned char ciphertext[1][32] = {
	{0x60,0x1e,0xc3,0x13,0x77,0x57,0x89,0xa5,0xb7,0xa7,0xf5,0x04,0xbb,0xf3,0xd2,0x28,0xf4,0x43,0xe3,0xca,0x4d,0x62,0xb5,0x9a,0xca,0x84,0xe9,0x90,0xca,0xca,0xf5,0xc5}
	};
	unsigned char iv[1][16] = {
	{0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff},
	};
	unsigned char key[1][32] = {
	{0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4}
	};

	unsigned char decrypted_text[32];
	//END DO NOT MODIFY

	//YOUR CODE HERE: Implement AES encryption, write the encrypted output of plaintext to enc_buf

	//needed for decryption
	encrypt_aes_ctr(plaintext[0], s, iv[0], key[0], enc_buf);

	printf("Plain text: ");	
	for(int i = 0; i < 32; i++){
		printf("%x", plaintext[0][i]);
	}
	printf("\n");

	printf("Encrypted data: ");
	for(int i = 0; i < 32; i++){
		printf("%x", enc_buf[i]);
	}
	printf("\n");

	printf("Cipher text: ");
	for(int i = 0; i < 32; i++){
		printf("%x", ciphertext[0][i]);
	}
	printf("\n");
	
	//YOUR CODE HERE: Implement AES decryption, write the decrypted output of enc_buf to decrypted_text

	decrypt_aes_ctr(enc_buf, decrypted_text, s, iv[0], key[0]);

	printf("Decrypted data: ");
	for(int i = 0; i < 32; i++){
		printf("%x", decrypted_text[i]);
	}
	printf("\n");

	//DO NOT MODIFY
	assert(memcmp(enc_buf, ciphertext[0], 32) == 0);
	assert(memcmp(decrypted_text, plaintext[0], 32) == 0);
	return 0;
}



/*
 * Substituting the value for state is simply indexing into the S box according to the original value
 */
void substitute_bytes(unsigned char state[4][4], unsigned char * s){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			//column major 
			state[j][i] = s[state[j][i]];
		}
	}
}

/*
 * Each row is shifted left a number of times equivalent to its index (Row 2 is shifted left 2 times)
 */
void shift_rows(unsigned char state[4][4]){

	unsigned char temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;

	temp = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = temp;

	temp = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = temp;

	temp = state[3][3];
	state[3][3] = state[3][2];
	state[3][2] = state[3][1];
	state[3][1] = state[3][0];
	state[3][0] = temp;

}

void rotate_word(unsigned char word[4]){
	unsigned char temp = word[0];
	word[0] = word[1];
	word[1] = word[2];
	word[2] = word[3];
	word[3] = temp;
}

void substitute_word(unsigned char word[4], unsigned char * s){
	for(int i = 0; i < 4; i++){
		word[i] = s[word[i]];
	}
}

unsigned char rcon[11] = {
	0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

void key_expansion(unsigned char * key, unsigned char w[60][4], unsigned char * s){
	for(int i = 0; i < 8; i++){
		w[i][0] = key[i*4];
		w[i][1] = key[i*4 + 1];
		w[i][2] = key[i*4 + 2];
		w[i][3] = key[i*4 + 3];
	}

	for(int i = 8; i < 60; i++){
		unsigned char temp[4];
		temp[0] = w[i-1][0];	
		temp[1] = w[i-1][1];
		temp[2] = w[i-1][2];
		temp[3] = w[i-1][3];

		if(i % 8 == 0){
			rotate_word(temp);
			substitute_word(temp, s);
			temp[0] ^= rcon[i/8];
		} else if(i % 8 == 4){
			substitute_word(temp, s);
		}
	
		w[i][0] = w[i-8][0] ^ temp[0];	
		w[i][1] = w[i-8][1] ^ temp[1];
		w[i][2] = w[i-8][2] ^ temp[2];
		w[i][3] = w[i-8][3] ^ temp[3];
	}
}

void add_round_key(unsigned char state[4][4], unsigned char round_key[4][4]){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			state[i][j] ^= round_key[i][j];
		}
	}
}

void sub_bytes(unsigned char state[4][4], unsigned char * s){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			state[i][j] = s[state[i][j]];
		}
	}
}

unsigned char gmul2(unsigned char a){
	return (a & 0x80) ? ((a << 1) ^ 0x1B) : (a << 1);
}

unsigned char gmul3(unsigned char a){
	return gmul2(a) ^ a;
}

void mix_columns(unsigned char state[4][4]){
	unsigned char temp[4];

	for(int i = 0; i < 4; i++){
		temp[0] = state[0][i];
		temp[1] = state[1][i];
		temp[2] = state[2][i];
		temp[3] = state[3][i];

		state[0][i] = gmul2(temp[0]) ^ gmul3(temp[1]) ^ temp[2] ^ temp[3];
		state[1][i] = temp[0] ^ gmul2(temp[1]) ^ gmul3(temp[2]) ^ temp[3];
		state[2][i] = temp[0] ^ temp[1] ^ gmul2(temp[2]) ^ gmul3(temp[3]);
		state[3][i] = gmul3(temp[0]) ^ temp[1] ^ temp[2] ^ gmul2(temp[3]);
	}
}

void print_state(unsigned char state[4][4]){
	printf("state: ");
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			printf("%x", state[i][j]);
		}
	}
	printf("\n");
}

/*
 * 1. Requires expanding the original key into expanded words (32 bits). We require one subkey (expanded word) for every round required by AES-256 which is 14. This is the Rjindael key schedule algorithm.
 * 2. For each round excluding the first and last round, we need to:
 * 	a. Substitute each byte in the input with a value from the S box.
 * 	b. Shift each row in the current "state" by the index of the row to the left (Row 2 is shifted left twice).
 * 	c. Each column is then "mixed" by applying matrix multiplication
 * 	d. Each state is then XORed with the expanded round key
 * 3. On the last round, we apply every step above, excluding the column mixing.
 * 4. The final state is the ciphertext block which is actually the encrypted counter/IV combo.
 */
void encrypt_aes(unsigned char * input, unsigned char * output, unsigned char * key, unsigned char * s_box){
	unsigned char state[4][4];
	unsigned char words[60][4];

	key_expansion(key, words, s_box);

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			state[j][i] = input[i*4 + j];
		}
	}

	unsigned char round_key[4][4];
	for(int i = 0; i < 4; i++){
		round_key[0][i] = words[i][0];
		round_key[1][i] = words[i][1];
		round_key[2][i] = words[i][2];
		round_key[3][i] = words[i][3];
	}

	add_round_key(state, round_key);

	
	for(int round = 1; round < 14; round++){
		substitute_bytes(state, s_box);
		shift_rows(state);
		mix_columns(state);

		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				round_key[j][i] = words[round*4 + i][j];
			}
		}

		add_round_key(state, round_key);

	}

	substitute_bytes(state, s_box);
	shift_rows(state);

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			round_key[j][i] = words[14*4 + i][j];
		}
	}

	add_round_key(state, round_key);


	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			output[i*4 + j] = state[j][i];
		}
	}
}

void increment_counter(unsigned char counter[16]){
	for(int i = 15; i >= 0; i--){
		if(++counter[i] != 0){
			break;
		}
	}	
}
/*
	We merely apply the same algorithm as encrypt_aes_ctr to decrypt, only this time the output is the result of XORing the encrypted text with the same encrypted counter
*/
void decrypt_aes_ctr(char * encrypted_text, char * decrypted_text, char * s_box, char * iv, char * key){
	unsigned char counter[16];
	memcpy(counter, iv, 16);

	unsigned char keystream[16];

	for(int i = 0; i < 32; i += 16){
		encrypt_aes(counter, keystream, key, s_box);
	
		int block_size = (i + 16 <= 32) ? 16 : (32 - i);
		for(int j = 0; j < block_size; j++){
			decrypted_text[i + j] = encrypted_text[i + j] ^ keystream[j];
		}

		increment_counter(counter);
	}
}
/*
 * To enable encryption for AES in CTR mode, we encrypt the initialization vector concatenated with a counter instead of encrypting the plaintext. Once we encrypt this counter/IV, we XOR
 * the result with the plaintext to acquire the encrypted plaintext.
 * This allows us to handle plaintext that has not been padded to align to 128 bits and handle plaintext of any length.
 */
void encrypt_aes_ctr(char * plaintext, char * s_box, char * iv, char * key, char * enc_buf){
	unsigned char counter[16];
	memcpy(counter, iv, 16);

	unsigned char keystream[16];
	
	for(int i = 0; i < 32; i += 16){
		encrypt_aes(counter, keystream, key, s_box);

		int block_size = (i + 16 <= 32) ? 16 : (32 - i);
		for(int j = 0; j < block_size; j++){
			enc_buf[i + j] = plaintext[i + j] ^ keystream[j];
		}

		increment_counter(counter);
	}

}

