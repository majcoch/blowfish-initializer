/*
 * blowfish_data.h
 *
 * Created: 26.05.2021 15:48:05
 *  Author: Micha³ Granda
 */ 


#ifndef BLOWFISH_DATA_H_
#define BLOWFISH_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifdef __AVR__
	#include <avr/pgmspace.h>
	#define FLASH_MEM PROGMEM
	#define MODIFY_ARR const
#else
	#define FLASH_MEM
	#define MODIFY_ARR
#endif // __AVR__

extern MODIFY_ARR uint32_t blowfish_p_box[18];

extern MODIFY_ARR uint32_t blowfish_s_box[4][256];

#ifdef __cplusplus
}
#endif

#endif /* BLOWFISH_DATA_H_ */