/*
 * effect_fuzz.c
 *
 *  Created on: Nov 19, 2018
 *      Author: Cleissom
 */

#include "effect_fuzz.h"

void effect_fuzz_init(
	effect_instance_fuzz * S,
	float32_t gain,
	float32_t mix)
{
	S->gain = gain;
	S->mix = mix;
}

#define M_E 2.71828

void effect_fuzz(
	effect_instance_fuzz * S,
	float32_t * pSrc,
	float32_t * pDst,
	uint16_t blockSize)
{
	uint16_t i;
	float32_t input, exponential;

	for(i = 0; i<blockSize; i++){
		input = pSrc[i];

		if(input > 0){
			exponential = 1.0 - exp(-input);
		} else {
			exponential = -1.0 + exp(input);
		}
		pDst[i] = (S->mix*exponential) + ((1 - S->mix)*input);
	}

/*	for(i = 0; i<blockSize; i++){
		input = pSrc[i];
		if(input != 0){
			q = input/(fabs(input));
			exponential = q*(1-exp(S->gain*(q*input)));
			pDst[i] = (S->mix*exponential) + ((1 - S->mix)*input);
		} else {
			pDst[i] = 0;
		}
	}*/
}

void effect_fuzz_set_gain(
		effect_instance_fuzz * S,
		float32_t gain)
{
	S->gain = gain;
}

float32_t effect_fuzz_get_gain(
		effect_instance_fuzz * S)
{
	return S->gain;
}

void effect_fuzz_set_mix(
		effect_instance_fuzz * S,
		float32_t mix)
{
	S->mix = mix;
}

float32_t effect_fuzz_get_mix(
		effect_instance_fuzz * S)
{
	return S->mix;
}
