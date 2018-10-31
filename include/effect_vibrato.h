/*
 * effect_vibrato.h
 *
 *  Created on: Oct 31, 2018
 *      Author: Cleissom
 */

#ifndef EFFECT_VIBRATO_H_
#define EFFECT_VIBRATO_H_

#include <arm_math.h>

#define SAMPLING_FREQUENCY 48000U

typedef struct {
	float32_t delay;     	/**< max delay of filter in ms. */
	float32_t alpha;		/**< coefficient for multiplication*/
	float32_t *pState;    	/**< points to the state variable array. The array is of length numTaps+blockSize-1. */
	uint16_t pStateSize;    /**< size of pState. */
	uint16_t head;
	uint16_t cosineIndex;			/**< Number of indexes that the tail is, relative to head. */
	float32_t vibratoDigitalFrequency;
} effect_instance_vibrato;

void effect_vibrato_init( effect_instance_vibrato * S, float32_t delay, float32_t alpha, float32_t frequency, float32_t * pState, uint16_t pStateSize);

#endif /* EFFECT_VIBRATO_H_ */
