/*
 * effect_overdrive.c
 *
 *  Created on: Nov 19, 2018
 *      Author: Cleissom
 */


#include "effect_overdrive.h"


void effect_overdrive(
	float32_t * pSrc,
	float32_t * pDst,
	uint16_t blockSize)
{
	uint16_t i;
	float32_t input;

	for(i = 0; i<blockSize; i++){
		input = pSrc[i];
		if(input != 0){
			if(fabs(input) < 1.0/3.0){
				pDst[i] = 2*input;
			} else if (fabs(input) < 2.0/3.0){
				pDst[i] = (3.0-(powf(2.0-3.0*input,2)))/3.0;
			} else {
				pDst[i] = 1;
			}
		} else {
			pDst[i] = 0;
		}
	}
}
