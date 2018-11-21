/*
 * effect_delay.h
 *
 *  Created on: Oct 29, 2018
 *      Author: Cleissom
 */

/**
 * @file effect_delay.h
 * Delay Effect
 */


#ifndef EFFECT_DELAY_H_
#define EFFECT_DELAY_H_


#include <arm_math.h>
#include "effect_common.h"

/**
 * @brief Instance structure for the Delay Effect.
 */

typedef struct {
	float32_t delay;     	/**< delay of filter in ms. */
	float32_t alpha;		/**< Scaling factor applied to the delayed signal.*/
	float32_t *pState;    	/**< points to the state variable array. The array is of length numTaps+blockSize-1. */
	uint16_t pStateSize;    /**< size of pState. */
	uint16_t head;			/**< position on state buffer. */
	uint16_t aheadIndex;	/**< Number of indexes that the tail is, relative to head. */
} effect_instance_delay;


/**
 * @brief Initialization function for the Delay Effect.
 *
 * @param *S points to an instance of the delay filter structure.
 * @param delay Number of filter delay in ms.
 * @param alpha Scaling factor applied to the delayed signal.
 * @param *pState ponts to the state buffer.
 * @param pStateSize Size of the state buffer.
 * @return none.
 *
 * \par Description
 * pStateSize is given by the equation:
 * 	<pre>
 * 		pStateSize = (delayMax * SAMPLING_FREQUENCY)/1000 + 1.
 * 	</pre>
 * or by:
 *  <pre>
 *		delayMax = ((pStateSize-1) * 1000) / SAMPLING_FREQUENCY, in ms.
 *  </pre>
 */

void effect_delay_init(effect_instance_delay *S, float32_t delay, float32_t alpha, float32_t *pState, uint16_t pStateSize);

/**
 * @brief Processing function for Delay Effect.
 * @param *S points to an instance of the delay filter structure.
 * @param *pSrc points to the souce buffer for filtering.
 * @param *pDst points to the destination buffer.
 * @param blockSize size of the block to be processed. Same size of the source/destination buffer.
 */
void effect_delay(effect_instance_delay * S, float32_t * pSrc, float32_t * pDst, uint16_t blockSize);
void effect_delay_set_delay(effect_instance_delay * S,  float32_t delay);
void effect_delay_set_alpha(effect_instance_delay * S,  float32_t alpha);

#endif /* EFFECT_DELAY_H_ */

