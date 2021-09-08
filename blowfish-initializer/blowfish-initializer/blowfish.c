/*
 * blowfish.c
 *
 * Created: 26.05.2021 19:25:46
 *  Author: Micha³ Granda
 */ 
#include "blowfish.h"
#include "blowfish_data_init.h"

#ifdef __AVR__
	#include <avr/pgmspace.h>
	#define FLASH_ACCESS(x) pgm_read_dword(&(x))
#else
	#define FLASH_ACCESS(x) (x)
#endif // __AVR__

#define GET_L(x) (uint32_t)(x >> 32)
#define GET_R(x) ((uint32_t)x)

uint32_t blowfish_f(uint32_t x) {
	uint32_t h = FLASH_ACCESS(blowfish_s_box[0][x >> 24]) + FLASH_ACCESS(blowfish_s_box[1][x >> 16 & 0xff]);
	return (h ^ FLASH_ACCESS(blowfish_s_box[2][x >> 8 & 0xff])) + FLASH_ACCESS(blowfish_s_box[3][x & 0xff]);
}

void blowfish_create_block(const uint8_t* data, uint64_t* block) {
	uint8_t shift = 56;
	*block = 0;
	for (uint8_t i = 0; i < 8; i++) {
		*block |= (uint64_t)(data[i]) << shift;
		shift -= 8;
	}
}

void blowfish_destroy_block(uint8_t* data, const uint64_t* block) {
	uint8_t shift = 56;
	for (uint8_t i = 0; i < 8; i++) {
		data[i] = (uint8_t)(*block >> shift);
		shift -= 8;
	}
}

#ifndef __AVR__

void blowfish_init(uint32_t* key, uint8_t key_len) {
	for (uint8_t i = 0; i < 18; ++i) {
		blowfish_p_box[i] ^= key[i % key_len];
	}

	uint64_t block = 0;
	for (uint8_t i = 0; i < 18; i += 2) {
		block = blowfish_encrypt_block(block);
		blowfish_p_box[i] = GET_L(block);
		blowfish_p_box[i + 1] = GET_R(block);
	}
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 256; j += 2) {
			block = blowfish_encrypt_block(block);
			blowfish_s_box[i][j] = GET_L(block);
			blowfish_s_box[i][j + 1] = GET_R(block);
		}
	}
}

#endif // ENABLE_INIT

uint64_t blowfish_encrypt_block(uint64_t block) {
	uint32_t L = (block >> 32);
	uint32_t R = (uint32_t)(block);
	
	for (uint8_t i = 0; i < 16; i += 2) {
		L ^= FLASH_ACCESS(blowfish_p_box[i]);
		R ^= blowfish_f(L);
		R ^= FLASH_ACCESS(blowfish_p_box[i + 1]);
		L ^= blowfish_f(R);
	}
	L ^= FLASH_ACCESS(blowfish_p_box[16]);
	R ^= FLASH_ACCESS(blowfish_p_box[17]);

	uint32_t tmp = L;
	L = R;
	R = tmp;
	
	return ((uint64_t)L << 32) | R;
}

void blowfish_encrypt(uint8_t* data, const size_t len) {
	for (size_t i = 0; i < len; i += 8) {
		uint64_t block = 0;
		blowfish_create_block(&data[i], &block);
		block = blowfish_encrypt_block(block);
		blowfish_destroy_block(&data[i], &block);
	}
}

uint64_t blowfish_decrypt_block(uint64_t block) {
	uint32_t L = (block >> 32);
	uint32_t R = (uint32_t)(block);
	
	for (int8_t i = 16; i > 0; i -= 2) {
		L ^= FLASH_ACCESS(blowfish_p_box[i + 1]);
		R ^= blowfish_f(L);
		R ^= FLASH_ACCESS(blowfish_p_box[i]);
		L ^= blowfish_f(R);
	}
	L ^= FLASH_ACCESS(blowfish_p_box[1]);
	R ^= FLASH_ACCESS(blowfish_p_box[0]);

	uint32_t tmp = L;
	L = R;
	R = tmp;
	
	return ((uint64_t)L << 32) | R;
}

void blowfish_decrypt(uint8_t* data, const size_t len) {
	for (size_t i = 0; i < len; i += 8) {
		uint64_t block = 0;
		blowfish_create_block(&data[i], &block);
		block = blowfish_decrypt_block(block);
		blowfish_destroy_block(&data[i], &block);
	}
}
