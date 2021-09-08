/*
 * blowfish.h
 *
 * Created: 26.05.2021 15:45:27
 *  Author: Micha³ Granda
 */ 


#ifndef BLOWFISH_H_
#define BLOWFISH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#ifndef __AVR__

void blowfish_init(uint32_t* key, uint8_t key_len);

#endif // !__AVR__

uint64_t blowfish_encrypt_block(uint64_t block);

void blowfish_encrypt(uint8_t* data, const size_t len);

uint64_t blowfish_decrypt_block(uint64_t block);

void blowfish_decrypt(uint8_t* data, const size_t len);

#ifdef __cplusplus
}
#endif

#endif /* BLOWFISH_H_ */