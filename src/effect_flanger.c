/*
 * effect_flanger.c
 *
 *  Created on: Nov 21, 2018
 *      Author: Cleissom
 */


#include "effect_flanger.h"

void effect_flanger_init(
	effect_instance_flanger * S,
	float32_t delay,
	float32_t gain,
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

	S->gain = gain;
	S->pState = pState;

	S->flangerDigitalFrequency = 2 * PI * (frequency / ((float32_t)SAMPLING_FREQUENCY));
}

void effect_flanger(
	effect_instance_flanger * S,
	float32_t * pSrc,
	float32_t * pDst,
	uint16_t blockSize)
{
	uint16_t i, aheadIndex;
	float32_t aheadIndexf32;
	float32_t flangerDelay;
	float32_t interpolation, x1, x2, frac;

	for(i = 0; i<blockSize; i++){
		flangerDelay = (S->delay/2) * (1 - arm_cos_f32(S->flangerDigitalFrequency * S->cosineIndex++));

		aheadIndexf32 = S->pStateSize - (flangerDelay * (float32_t)(SAMPLING_FREQUENCY/1000U));
		aheadIndex = aheadIndexf32;

		x1 = (S->pState[(S->head + (aheadIndex-1)) % (S->pStateSize)]);
		x2 = (S->pState[(S->head + (aheadIndex-2)) % (S->pStateSize)]);
		frac = ((uint32_t)aheadIndex) - aheadIndex;
		interpolation = x1*frac + x2*(1-frac);
		pDst[i] = (S->gain * interpolation) ;

		S->pState[(S->head)++] = pSrc[i];

		if (S->head >= S->pStateSize)
			S->head = 0;

		if (S->cosineIndex >= SAMPLING_FREQUENCY)
			S->cosineIndex = 0;
	}
}

void effect_flanger_set_delay(
		effect_instance_flanger * S,
		float32_t delay)
{
	float32_t delayMax = (float32_t)((S->pStateSize-1) * 1000) / SAMPLING_FREQUENCY;

	if (delay > delayMax)
		S->delay = delayMax;
	else
		S->delay = delay;
}

float32_t effect_flanger_get_delay(
		effect_instance_flanger * S)
{
	return S->delay;
}

void effect_flanger_set_frequency(
		effect_instance_flanger * S,
		float32_t frequency)
{
	S->flangerDigitalFrequency = 2 * PI * (frequency / ((float32_t)SAMPLING_FREQUENCY));
}

float32_t effect_flanger_get_frequency(
		effect_instance_flanger * S)
{
	return (S->flangerDigitalFrequency * (float32_t)SAMPLING_FREQUENCY)/(2 * PI );
}

void effect_flanger_set_gain(
		effect_instance_flanger * S,
		float32_t gain)
{
	S->gain = gain;
}

float32_t effect_flanger_get_gain(
		effect_instance_flanger * S)
{
	return S->gain;
}
