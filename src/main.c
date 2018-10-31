#include <stm32f4xx.h>
#include <arm_math.h>
#include <stm32f4_discovery.h>
#include <stm32f4_discovery_accelerometer.h>
#include <wolfson_pi_audio.h>
#include <diag/Trace.h>
#include <tests.h>
#include <dwt.h>
#include "filter.h"
#include "effect_delay.h"
//#include "math_helper.h"


#define BLOCK_SIZE (WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE)/4
#define NUM_TAPS 502
#undef CYCLE_COUNTER
//#define CYCLE_COUNTER

int16_t TxBuffer[WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE];
int16_t RxBuffer[WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE];

__IO BUFFER_StateTypeDef buffer_offset = BUFFER_OFFSET_NONE;

__IO uint8_t Volume = 80;

uint32_t AcceleroTicks;
int16_t AcceleroAxis[3];


//Declare State buffer
#define DELAY_STATE_SIZE 10000
static float32_t delayState[DELAY_STATE_SIZE];

int main(int argc, char* argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	uint32_t cycleCount;
	uint32_t i, k;

	float32_t inputF32Buffer[BLOCK_SIZE];
	float32_t outputF32Buffer[BLOCK_SIZE];


#ifdef OS_USE_SEMIHOSTING
	//Semihosting example
	//FILE *CoefficientsFile;
	FILE *CycleFile;
	//float Coefficients[NUM_TAPS];
#endif

	// Initialise the HAL Library; it must be the first
	// instruction to be executed in the main program.
	HAL_Init();

	DWT_Enable();

#ifdef OS_USE_SEMIHOSTING
	//Semihosting example
	/*
	CoefficientsFile = fopen("coefficients.txt", "r");
	if (!CoefficientsFile) {
		trace_printf("Error trying to open CoefficientsFile. Check the name/location.\n");
		while(1);
	}

	for(i=0; i<NUM_TAPS; i++)
		fscanf(CoefficientsFile, "%f", &Coefficients[i]);

	for(i=0; i<NUM_TAPS; i++)
		trace_printf("Coefficient %d: %f\n", i, Coefficients[i]);

	fclose(CoefficientsFile);
	*/
#ifdef CYCLE_COUNTER
	CycleFile = fopen("cyclecounter.txt", "w");
	if (!CycleFile) {
		trace_printf("Error trying to open cycle counter file\n.");
		while(1);
	}
#endif

#endif

	WOLFSON_PI_AUDIO_Init((INPUT_DEVICE_LINE_IN << 8) | OUTPUT_DEVICE_BOTH, 80, AUDIO_FREQUENCY_48K);

	WOLFSON_PI_AUDIO_SetInputMode(INPUT_DEVICE_LINE_IN);

	WOLFSON_PI_AUDIO_SetMute(AUDIO_MUTE_ON);

	WOLFSON_PI_AUDIO_Play(TxBuffer, RxBuffer, WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE);

	WOLFSON_PI_AUDIO_SetVolume(Volume);

	BSP_ACCELERO_Init();

	TEST_Init();

	float32_t  *inputF32, *outputF32;

	/* Initialize input and output buffer pointers */
	inputF32 = &inputF32Buffer[0];
	outputF32 = &outputF32Buffer[0];


	effect_instance_delay effectDelay;
	effect_delay_init(&effectDelay, 200, 0.5, delayState, DELAY_STATE_SIZE);



	trace_printf("End of filter initialization.\n");

	while (1) {
		// Add your code here.
		if(buffer_offset == BUFFER_OFFSET_HALF)
		{
			DWT_Reset();

			cycleCount = DWT_GetValue();

			for(i=0, k=0; i<(WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE/2); i++) {
				if(i%2) {
					inputF32Buffer[k] = (float32_t)(RxBuffer[i]/32768.0);//convert to float LEFT
					k++;
				}
				else {
					TxBuffer[i] = RxBuffer[i];//   RIGHT (canal de baixo no OcenAudio)
				}
			}

			effect_delay(&effectDelay, inputF32, outputF32, BLOCK_SIZE);
			for(i=0, k=0; i<(WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE/2); i++) {
				if(i%2)	{
					TxBuffer[i] = (int16_t)(outputF32Buffer[k]*32768);//back to 1.15
					k++;
				}
			}


#ifdef CYCLE_COUNTER
			fprintf(CycleFile, "\nHALF: %lu", (DWT_GetValue()- cycleCount));
#endif

			buffer_offset = BUFFER_OFFSET_NONE;
		}

		if(buffer_offset == BUFFER_OFFSET_FULL)
		{
			DWT_Reset();

			cycleCount = DWT_GetValue();

			for(i=(WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE/2), k=0; i<WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE; i++) {
				if(i%2) {
					inputF32Buffer[k] = (float32_t)(RxBuffer[i]/32768.0);//convert to float
					k++;
				}
				else {
					TxBuffer[i] = RxBuffer[i];//pass-through(int16_t)0.3*32768;//
				}
			}

			effect_delay(&effectDelay, inputF32, outputF32, BLOCK_SIZE);
			for(i=(WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE/2), k=0; i<WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE; i++) {
				if(i%2)	{
					TxBuffer[i] = (int16_t)(outputF32Buffer[k]*32768.0);//back to 1.15
					k++;
				}
			}


#ifdef CYCLE_COUNTER
			fprintf(CycleFile, "\nFULL: %lu", (DWT_GetValue()- cycleCount));
#endif

			buffer_offset = BUFFER_OFFSET_NONE;
		}
		//TEST_Main();
	}
	fclose(CycleFile);
	return 0;
}

/*--------------------------------
Callbacks implementation:
--------------------------------------------------------*/

/**
  * @brief  Manages the DMA full Transfer complete event.
  */
void WOLFSON_PI_AUDIO_TransferComplete_CallBack(void)
{
	buffer_offset = BUFFER_OFFSET_FULL;
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  */
void WOLFSON_PI_AUDIO_HalfTransfer_CallBack(void)
{
	  buffer_offset = BUFFER_OFFSET_HALF;
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
void WOLFSON_PI_AUDIO_OUT_Error_CallBack(void)
{
  /* Stop the program with an infinite loop */
  while (1);
}
