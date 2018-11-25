/*
 * effect_vibrato.h
 *
 *  Created on: Oct 31, 2018
 *      Author: Cleissom
 */

#ifndef EFFECT_VIBRATO_H_
#define EFFECT_VIBRATO_H_

#include <arm_math.h>
#include "effect_common.h"

/**
 * @brief Instance structure for the Vibrato Effect.
 */
typedef struct {
	float32_t delay;     	/**< max delay of filter in ms. */
	float32_t gain;			/**< Scaling factor.*/
	float32_t *pState;    	/**< points to the state variable array. The array is of length numTaps+blockSize-1. */
	uint16_t pStateSize;    /**< size of pState. */
	uint16_t head;			/**< position on state buffer. */
	uint16_t cosineIndex;	/**< index number for digital frequency */
	float32_t vibratoDigitalFrequency;	/**< Digital frequency of vibrato oscilation */
} effect_instance_vibrato;



void effect_vibrato_init( effect_instance_vibrato * S, float32_t delay, float32_t gain, float32_t frequency, float32_t * pState, uint16_t pStateSize);
void effect_vibrato(effect_instance_vibrato * S, float32_t * pSrc, float32_t * pDst, uint16_t blockSize);
void effect_vibrato_set_delay(effect_instance_vibrato * S, float32_t delay);
void effect_vibrato_set_frequency(effect_instance_vibrato * S, float32_t frequency);
void effect_vibrato_set_gain(effect_instance_vibrato * S, float32_t gain);
float32_t effect_vibrato_get_delay(effect_instance_vibrato * S);
float32_t effect_vibrato_get_frequency(effect_instance_vibrato * S);
float32_t effect_vibrato_get_gain(effect_instance_vibrato * S);

#endif /* EFFECT_VIBRATO_H_ */
