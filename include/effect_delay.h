/*
 * effect_delay.h
 *
 *  Created on: Oct 29, 2018
 *      Author: Cleissom
 */

#ifndef EFFECT_DELAY_H_
#define EFFECT_DELAY_H_

#include <arm_math.h>

#define SAMPLING_FREQUENCY 48000U

typedef struct {
	uint16_t delay;     	/**< delay of filter in ms. */
	float32_t alpha;		/**< coefficient for multiplication*/
	float32_t *pState;    	/**< points to the state variable array. The array is of length numTaps+blockSize-1. */
	uint16_t pStateSize;    /**< size of pState. */
	uint16_t head;
	uint16_t tail;
} effect_instance_delay;

void effect_delay_init(effect_instance_delay * S, uint16_t delay, float32_t alpha, float32_t * pState, uint16_t pStateSize);
void effect_delay(effect_instance_delay * S, float32_t * pSrc, float32_t * pDst, uint16_t blockSize);

#endif /* EFFECT_DELAY_H_ */

