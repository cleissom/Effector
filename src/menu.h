/*
 * menu.h
 *
 *  Created on: Nov 24, 2018
 *      Author: Cleissom
 */

#ifndef MENU_H_
#define MENU_H_

#include <arm_math.h>


typedef enum {
	NONE,
	ECHO,
	VIBRATO,
	FLANGER,
	TREMOLO,
	FUZZ,
	OVERDRIVE
} filter_type_enum;


typedef enum {
	MENU_HOME,
	MENU_ECHO,
	MENU_ECHO_DELAY,
	MENU_ECHO_GAIN,
	MENU_VIBRATO,
	MENU_VIBRATO_DELAY,
	MENU_VIBRATO_FREQUENCY,
	MENU_VIBRATO_GAIN,
	MENU_FLANGER,
	MENU_FLANGER_DELAY,
	MENU_FLANGER_FREQUENCY,
	MENU_FLANGER_GAIN,
	MENU_TREMOLO,
	MENU_TREMOLO_FREQUENCY,
	MENU_TREMOLO_GAIN,
	MENU_FUZZ,
	MENU_FUZZ_GAIN,
	MENU_FUZZ_MIX,
	MENU_OVERDRIVE
} menu_state_enum;

void effect_init(void);
void effect(float32_t * pSrc, float32_t * pDst, uint16_t blockSize);
void updateScreen(void);
void menuValueAdd(void);
void menuValueSub(void);
void menuValueEnter(void);

#endif /* MENU_H_ */
