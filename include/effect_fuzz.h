/*
 * effect_fuzz.h
 *
 *  Created on: Nov 19, 2018
 *      Author: Cleissom
 */

#ifndef EFFECT_FUZZ_H_
#define EFFECT_FUZZ_H_

#include <arm_math.h>
#include "effect_common.h"

/**
 * @brief Instance structure for the Fuzz Effect.
 */
typedef struct {
	float32_t 	gain;		/**< Fuzz gain.*/
	float32_t 	mix;		/**< Mix of original and distorted sound. 1 only distorted */
} effect_instance_fuzz;


void effect_fuzz_init( effect_instance_fuzz * S, float32_t gain, float32_t mix);
void effect_fuzz(effect_instance_fuzz * S, float32_t * pSrc, float32_t * pDst, uint16_t blockSize);
void effect_fuzz_set_gain(effect_instance_fuzz * S, float32_t gain);
float32_t effect_fuzz_get_gain(effect_instance_fuzz * S);
void effect_fuzz_set_mix(effect_instance_fuzz * S, float32_t mix);
float32_t effect_fuzz_get_mix(effect_instance_fuzz * S);

#endif /* EFFECT_FUZZ_H_ */
