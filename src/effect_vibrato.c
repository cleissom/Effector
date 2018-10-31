/*
 * effect_vibrato.c
 *
 *  Created on: Oct 31, 2018
 *      Author: Cleissom
 */


/*
 * effect_delay.c
 *
 *  Created on: Oct 29, 2018
 *      Author: Cleissom
 */

#include "effect_vibrato.h"

void effect_vibrato_init(
	effect_instance_vibrato * S,
	float32_t delay,
	float32_t alpha,
	float32_t frequency,
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
	S->cosineIndex = 0;

	S->alpha = alpha;
	S->pState = pState;

	S->vibratoDigitalFrequency = 2 * PI * (frequency / ((float32_t)SAMPLING_FREQUENCY));
}

void effect_vibrato(
	effect_instance_vibrato * S,
	float32_t * pSrc,
	float32_t * pDst,
	uint16_t blockSize)
{
	uint16_t i, aheadIndex;
	float32_t aheadIndexf32;
	float32_t vibratoDelay;
	float32_t interpolation, x1, x2, frac;

	for(i = 0; i<blockSize; i++){
		vibratoDelay = (S->delay/2) * (1 - arm_cos_f32(S->vibratoDigitalFrequency * S->cosineIndex++));
		aheadIndexf32 = S->pStateSize - (vibratoDelay * (float32_t)(SAMPLING_FREQUENCY/1000U));
		aheadIndex = aheadIndexf32;
		x1 = (S->pState[(S->head + (aheadIndex-1)) % (S->pStateSize)]);
		x2 = (S->pState[(S->head + (aheadIndex-2)) % (S->pStateSize)]);
		frac = ((uint32_t)aheadIndex) - aheadIndex;
		interpolation = x1*frac + x2*(1-frac);
		pDst[i] = S->alpha * interpolation;

		S->pState[(S->head)++] = pSrc[i];

		if (S->head >= S->pStateSize)
			S->head = 0;

		if (S->cosineIndex >= SAMPLING_FREQUENCY)
			S->cosineIndex = 0;
	}
}

void effect_vibrato_set_delay(
		effect_instance_vibrato * S,
		float32_t delay)
{
	float32_t delayMax = (float32_t)((S->pStateSize-1) * 1000) / SAMPLING_FREQUENCY;

	if (delay > delayMax)
		S->delay = delayMax;
	else
		S->delay = delay;
}

void effect_vibrato_set_alpha(
		effect_instance_vibrato * S,
		float32_t alpha)
{
	S->alpha = alpha;
}
