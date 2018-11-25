/*
 * effect_tremolo.h
 *
 *  Created on: Nov 19, 2018
 *      Author: Cleissom
 */

#ifndef EFFECT_TREMOLO_H_
#define EFFECT_TREMOLO_H_

#include <arm_math.h>
#include "effect_common.h"

/**
 * @brief Instance structure for the Tremolo Effect.
 */
typedef struct {
	float32_t gain;		/**< Scaling factor.*/
	uint16_t cosineIndex;	/**< index number for digital frequency */
	float32_t tremoloDigitalFrequency;	/**< Digital frequency of tremolo oscilation */
} effect_instance_tremolo;



void effect_tremolo_init( effect_instance_tremolo * S, float32_t gain, float32_t frequency);
void effect_tremolo(effect_instance_tremolo * S, float32_t * pSrc, float32_t * pDst, uint16_t blockSize);
void effect_tremolo_set_gain(effect_instance_tremolo * S, float32_t gain);
float32_t effect_tremolo_get_gain(effect_instance_tremolo * S);
void effect_tremolo_set_frequency(effect_instance_tremolo * S, float32_t frequency);
float32_t effect_tremolo_get_frequency(effect_instance_tremolo * S);

#endif /* EFFECT_TREMOLO_H_ */
