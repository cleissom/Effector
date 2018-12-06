/*
 * effect_delay.c
 *
 *  Created on: Oct 29, 2018
 *      Author: Cleissom
 */

#include <effect_echo.h>

void effect_echo_init(
	effect_instance_echo * S,
	float32_t delay,
	float32_t gain,
	float32_t * pState,
	uint16_t pStateSize)
{
	S->pStateSize = pStateSize;

	float32_t delayMax = (float32_t)((S->pStateSize-1) * 1000) / SAMPLING_FREQUENCY;

	if (delay > delayMax)
		S->delay = delayMax;
	else
		S->delay = delay;

	S->head = 0;
	S->aheadIndex = (S->pStateSize - (uint16_t)(S->delay * (SAMPLING_FREQUENCY/1000U))) -1;

	S->gain = gain;
	S->pState = pState;
}

void effect_echo(
	effect_instance_echo * S,
	float32_t * pSrc,
	float32_t * pDst,
	uint16_t blockSize)
{
	uint16_t i;

	for(i = 0; i<blockSize; i++){
		pDst[i] = pSrc[i] + S->gain * (S->pState[(S->head + S->aheadIndex) % (S->pStateSize)]);
		S->pState[(S->head)++] = pSrc[i];

		if (S->head >= S->pStateSize)
			S->head = 0;
	}
}

void effect_echo_set_delay(
		effect_instance_echo * S,
		float32_t delay)
{
	float32_t delayMax = (float32_t)((S->pStateSize-1) * 1000) / SAMPLING_FREQUENCY;

	if (delay > delayMax)
		S->delay = delayMax;
	else
		S->delay = delay;

	S->aheadIndex = S->pStateSize - (uint16_t)(S->delay * (SAMPLING_FREQUENCY/1000U));
}

float32_t effect_echo_get_delay(
		effect_instance_echo * S)
{
	return S->delay;
}

void effect_echo_set_gain(
		effect_instance_echo * S,
		float32_t gain)
{
	S->gain = gain;
}

float32_t effect_echo_get_gain(
		effect_instance_echo * S)
{
	return S->gain;
}
