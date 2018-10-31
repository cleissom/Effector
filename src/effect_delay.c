/*
 * effect_delay.c
 *
 *  Created on: Oct 29, 2018
 *      Author: Cleissom
 */

#include "effect_delay.h"

void effect_delay_init(
	effect_instance_delay * S,
	uint16_t delay,
	float32_t alpha,
	float32_t * pState,
	uint16_t pStateSize)
{
	S->pStateSize = pStateSize;
	S->delay = delay;
	S->alpha = alpha;
	S->pState = pState;

	S->head = 0;
	S->tail = pStateSize - (S->delay * (SAMPLING_FREQUENCY/1000U));
}

void effect_delay(
	effect_instance_delay * S,
	float32_t * pSrc,
	float32_t * pDst,
	uint16_t blockSize)
{
	uint16_t i;

	for(i = 0; i<blockSize; i++){
		pDst[i] = pSrc[i] + S->alpha * (S->pState[(S->tail)++]);
		S->pState[(S->head)++] = pSrc[i];

		if (S->head >= S->pStateSize)
			S->head = 0;

		if (S->tail >= S->pStateSize)
			S->tail = 0;
	}
}
