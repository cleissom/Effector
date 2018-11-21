/*
 * effect_tremolo.c
 *
 *  Created on: Nov 19, 2018
 *      Author: Cleissom
 */

#include "effect_tremolo.h"

void effect_tremolo_init(
	effect_instance_tremolo * S,
	float32_t alpha,
	float32_t frequency)
{
	S->cosineIndex = 0;

	S->alpha = alpha;

	S->tremoloDigitalFrequency = 2 * PI * (frequency / ((float32_t)SAMPLING_FREQUENCY));
}

void effect_tremolo(
	effect_instance_tremolo * S,
	float32_t * pSrc,
	float32_t * pDst,
	uint16_t blockSize)
{
	uint16_t i;
	float32_t tremoloGain;

	for(i = 0; i<blockSize; i++){
		tremoloGain = (1 + S->alpha * arm_cos_f32(S->tremoloDigitalFrequency * S->cosineIndex++));

		pDst[i] = tremoloGain * pSrc[i];

		if (S->cosineIndex >= SAMPLING_FREQUENCY)
			S->cosineIndex = 0;
	}
}

void effect_tremolo_set_alpha(
		effect_instance_tremolo * S,
		float32_t alpha)
{
	S->alpha = alpha;
}

void effect_tremolo_set_frequency(
		effect_instance_tremolo * S,
		float32_t frequency)
{
	S->tremoloDigitalFrequency = 2 * PI * (frequency / ((float32_t)SAMPLING_FREQUENCY));
}
