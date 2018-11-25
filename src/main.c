#include <stm32f4xx.h>
#include <arm_math.h>
#include <stm32f4_discovery.h>
#include <stm32f4_discovery_accelerometer.h>
#include <wolfson_pi_audio.h>
#include <diag/Trace.h>
#include <dwt.h>
#include <effect_echo.h>
#include "filter.h"
#include "effect_vibrato.h"
#include "effect_flanger.h"
#include "effect_tremolo.h"
#include "effect_fuzz.h"
#include "effect_overdrive.h"
#include <string.h>
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "menu.h"

//#include "math_helper.h"


#define BLOCK_SIZE (WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE)/4

#undef CYCLE_COUNTER
//#define CYCLE_COUNTER

I2C_HandleTypeDef hi2c1;

int16_t TxBuffer[WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE];
int16_t RxBuffer[WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE];

__IO BUFFER_StateTypeDef buffer_offset = BUFFER_OFFSET_NONE;

__IO uint8_t Volume = 80;

uint32_t AcceleroTicks;
int16_t AcceleroAxis[3];

GPIO_PinState encoderLastVal = GPIO_PIN_RESET;
GPIO_PinState encoderNowVal = GPIO_PIN_RESET;
uint32_t encoderValue = 0;
const uint32_t encoderDebounceTime = 5; // in ms
const uint32_t pushButtonDebounceTime = 500;

uint32_t encoderLastTick;
uint32_t pushButtonLastTick;
char encoderValueStr[10];




void MX_I2C1_Init(void);
void MX_GPIO_Init(void);

int main(int argc, char* argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	uint32_t i, k;

	float32_t inputF32Buffer[BLOCK_SIZE];
	float32_t outputF32Buffer[BLOCK_SIZE];

	// Initialise the HAL Library; it must be the first
	// instruction to be executed in the main program.
	HAL_Init();

	DWT_Enable();

#ifdef OS_USE_SEMIHOSTING
#ifdef CYCLE_COUNTER
	FILE *CycleFile;
	uint32_t cycleCount;
	CycleFile = fopen("cyclecounter.txt", "w");
	if (!CycleFile) {
		trace_printf("Error trying to open cycle counter file\n.");
		while(1);
	}

	// DWT_Reset();

	// cycleCount = DWT_GetValue();

#endif
#endif

	WOLFSON_PI_AUDIO_Init((INPUT_DEVICE_LINE_IN << 8) | OUTPUT_DEVICE_BOTH, 80, AUDIO_FREQUENCY_48K);

	WOLFSON_PI_AUDIO_SetInputMode(INPUT_DEVICE_LINE_IN);

	WOLFSON_PI_AUDIO_SetMute(AUDIO_MUTE_ON);

	WOLFSON_PI_AUDIO_Play(TxBuffer, RxBuffer, WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE);

	WOLFSON_PI_AUDIO_SetVolume(Volume);

	BSP_ACCELERO_Init();

	MX_GPIO_Init();

	MX_I2C1_Init();

	ssd1306_Init();

	effect_init();

	//ssd1306_TestAll();

	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);

	float32_t  *inputF32, *outputF32;

	/* Initialize input and output buffer pointers */
	inputF32 = &inputF32Buffer[0];
	outputF32 = &outputF32Buffer[0];



	trace_printf("End of initialization.\n");

	while (1) {
		if(buffer_offset == BUFFER_OFFSET_HALF)
		{
			for(i=0, k=0; i<(WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE/2); i++) {
				if(i%2) {
					inputF32Buffer[k] = (float32_t)(RxBuffer[i]/32768.0);//convert to float LEFT
					k++;
				}
				else {
					TxBuffer[i] = RxBuffer[i];//   RIGHT (canal de baixo no OcenAudio)
				}
			}

			effect(inputF32, outputF32, BLOCK_SIZE);
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

			updateScreen();
		}

		if(buffer_offset == BUFFER_OFFSET_FULL)
		{
			DWT_Reset();
			//cycleCount = DWT_GetValue();

			for(i=(WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE/2), k=0; i<WOLFSON_PI_AUDIO_TXRX_BUFFER_SIZE; i++) {
				if(i%2) {
					inputF32Buffer[k] = (float32_t)(RxBuffer[i]/32768.0);//convert to float
					k++;
				}
				else {
					TxBuffer[i] = RxBuffer[i];//pass-through(int16_t)0.3*32768;//
				}
			}

			effect(inputF32, outputF32, BLOCK_SIZE);
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

/* I2C1 init function */
 void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
//  hi2c1.Init.ClockSpeed = 100000; // 9 FPS
//  hi2c1.Init.ClockSpeed = 100000 * 2; // 19 FPS
  hi2c1.Init.ClockSpeed = 100000 * 4; // 37 FPS
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  GPIO_InitTypeDef GPIO_InitStruct;

	/**I2C1 GPIO Configuration
	PB8     ------> I2C1_SCL
	PB9     ------> I2C1_SDA
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	__HAL_RCC_I2C1_CLK_ENABLE();

	HAL_I2C_Init(&hi2c1);
}

 void MX_GPIO_Init(void){

	 GPIO_InitTypeDef GPIO_InitStruct;

	 __HAL_RCC_GPIOB_CLK_ENABLE();

	  /*Configure GPIO pins : PB4 PB5 */
	  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  /*Configure GPIO pin : PB7 */
	  GPIO_InitStruct.Pin = GPIO_PIN_7;
	  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  /* EXTI interrupt init*/
	  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

 }




 void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if((GPIO_Pin == GPIO_PIN_4) || (GPIO_Pin == GPIO_PIN_5)){
		if(HAL_GetTick() > (encoderLastTick + encoderDebounceTime)){
			GPIO_PinState encoderB = GPIO_PIN_RESET;

			encoderNowVal = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
			encoderB = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);

			if(encoderNowVal == GPIO_PIN_SET){
				BSP_LED_On(LED3);
			} else {
				BSP_LED_Off(LED3);
			}

			if(encoderB == GPIO_PIN_SET){
				BSP_LED_On(LED4);
			} else {
				BSP_LED_Off(LED4);
			}

			if(encoderNowVal != encoderLastVal){
				if(encoderB != encoderNowVal){
					menuValueAdd();
				} else {
					menuValueSub();
				}
			}


			encoderLastTick = HAL_GetTick();
			encoderLastVal = encoderNowVal;
		}

	}

	else if(GPIO_Pin == GPIO_PIN_7)
		if(HAL_GetTick() > (pushButtonLastTick + pushButtonDebounceTime)){
			menuValueEnter();
			pushButtonLastTick = HAL_GetTick();
			BSP_LED_Toggle(LED5);
		}

}
