/*
 * effect_overdrive.h
 *
 *  Created on: Nov 19, 2018
 *      Author: Cleissom
 */

#ifndef EFFECT_OVERDRIVE_H_
#define EFFECT_OVERDRIVE_H_

#include <arm_math.h>
#include "effect_common.h"

void effect_overdrive(float32_t * pSrc, float32_t * pDst, uint16_t blockSize);

#endif /* EFFECT_OVERDRIVE_H_ */
