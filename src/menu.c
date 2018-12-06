/*
 * menu.c
 *
 *  Created on: Nov 24, 2018
 *      Author: Cleissom
 */

#include <dwt.h>

#include "menu.h"


#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "effect_echo.h"
#include "effect_vibrato.h"
#include "effect_flanger.h"
#include "effect_tremolo.h"
#include "effect_fuzz.h"
#include "effect_overdrive.h"

// Effects Instances
effect_instance_echo 	SEcho;
effect_instance_vibrato SVibrato;
effect_instance_flanger SFlanger;
effect_instance_tremolo STremolo;
effect_instance_fuzz	SFuzz;

// Effects Strings
char effectorStr[] 	= "Effector";
char effectStr[] 	= "Effect";
char backStr[]		= "Back";
char gainStr[] 		= "Gain";
char frequencyStr[]	= "Frequency";
char mixStr[]		= "Mix";
char delayStr[] 	= "Delay";
char echoStr[]		= "Echo";
char vibratoStr[] 	= "Vibrato";
char flangerStr[]	= "Flanger";
char tremoloStr[]	= "Tremolo";
char overdriveStr[]	= "Overdrive";
char msStr[]		= "ms";
char hzStr[]		= "Hz";

menu_state_enum menu_state = MENU_HOME;

filter_type_enum filter_type = NONE;

uint8_t enter = 0;

//Declare State buffer
#define DELAY_STATE_SIZE 10000
static float32_t State[DELAY_STATE_SIZE];

uint32_t Value;
char ValueStr[10];

void menuValueAdd(void){
	Value++;
}

void menuValueSub(void){
	if(Value != 0)
		Value--;
}

void menuValueEnter(void){

	enter = 1;

}

void effect_init(void){
	effect_echo_init(&SEcho, 0, 1, State, DELAY_STATE_SIZE);
	effect_vibrato_init(&SVibrato, 0, 1, 0, State, DELAY_STATE_SIZE);
	effect_flanger_init(&SFlanger, 0, 1, 0, State, DELAY_STATE_SIZE);
	effect_tremolo_init(&STremolo, 0, 0);
	effect_fuzz_init(&SFuzz, 0, 0);
}

void effects_pass_through(float32_t * pSrc, float32_t * pDst, uint16_t blockSize){
	uint16_t i;

	for(i = 0; i<blockSize; i++){
		pDst[i] = pSrc[i];
	}
}

void effect(float32_t * pSrc, float32_t * pDst, uint16_t blockSize){

	switch(filter_type){
	case ECHO:
		effect_echo(&SEcho, pSrc, pDst, blockSize);
		break;

	case VIBRATO:
		effect_vibrato(&SVibrato, pSrc, pDst, blockSize);
		break;

	case FLANGER:
		effect_flanger(&SFlanger, pSrc, pDst, blockSize);
		break;

	case TREMOLO:
		effect_tremolo(&STremolo, pSrc, pDst, blockSize);
		break;

	case OVERDRIVE:
		effect_overdrive(pSrc, pDst, blockSize);
		break;

	case NONE:
		effects_pass_through(pSrc, pDst, blockSize);
		break;
	}
}

void menuPrintLines(char* firstLine, char* secondLine, char* unity){
	ssd1306_SetCursor(0,0);
	ssd1306_Fill(Black);
	ssd1306_WriteString(firstLine, Font_11x18, White);
	ssd1306_WriteChar(':', Font_11x18, White);
	ssd1306_SetCursor(0,30);
	ssd1306_WriteString(secondLine, Font_16x26, White);
	if(unity != NULL){
		ssd1306_SetCursor(70,30);
		ssd1306_WriteString(unity, Font_16x26, White);
	}
	ssd1306_UpdateScreen();
}

void menuEnter(menu_state_enum state, filter_type_enum filter, uint32_t value){
	menu_state = state;
	filter_type = filter;
	Value = value;
	enter = 0;
}


void updateScreen(void){

	switch(menu_state){
	case MENU_HOME:

		if(Value > 4){
			Value = 4;
		}

		switch(Value){
		case 0:
			menuPrintLines(effectStr, echoStr, NULL);
			if(enter)
				menuEnter(MENU_ECHO, ECHO, 0);
			break;

		case 1:
			menuPrintLines(effectStr, vibratoStr, NULL);
			if(enter)
				menuEnter(MENU_VIBRATO, VIBRATO, 0);
			break;

		case 2:
			menuPrintLines(effectStr, flangerStr, NULL);
			if(enter)
				menuEnter(MENU_FLANGER, FLANGER, 0);
			break;

		case 3:
			menuPrintLines(effectStr, tremoloStr, NULL);
			if(enter)
				menuEnter(MENU_TREMOLO, TREMOLO, 0);
			break;

		case 4:
			menuPrintLines(effectStr, overdriveStr, NULL);
			if(enter)
				menuEnter(MENU_OVERDRIVE, OVERDRIVE, 0);
			break;
		}
		break;




		case MENU_ECHO:

			if(Value > 2){
				Value = 2;
			}
			switch(Value){
			case 0:
				menuPrintLines(echoStr, delayStr, NULL);
				if(enter)
					menuEnter(MENU_ECHO_DELAY, ECHO, effect_echo_get_delay(&SEcho));

				break;

			case 1:
				menuPrintLines(echoStr, gainStr, NULL);
				if(enter)
					menuEnter(MENU_ECHO_GAIN, ECHO, 10 * effect_echo_get_gain(&SEcho));
				break;

			case 2:
				menuPrintLines(echoStr, backStr, NULL);
				if(enter)
					menuEnter(MENU_HOME, NONE, 0);
				break;
			}
			break;

		case MENU_ECHO_DELAY:
			effect_echo_set_delay(&SEcho, Value);
			sprintf(ValueStr, "%lu", Value);
			menuPrintLines(delayStr, ValueStr, msStr);
			if(enter)
				menuEnter(MENU_ECHO, ECHO, 0);
			break;

		case MENU_ECHO_GAIN:
			effect_echo_set_gain(&SEcho, ((float32_t)Value)/10.0);
			sprintf(ValueStr, "%lu", Value);
			menuPrintLines(gainStr, ValueStr, NULL);
			if(enter)
				menuEnter(MENU_ECHO, ECHO, 1);
			break;




		case MENU_VIBRATO:

			if(Value > 3){
				Value = 3;
			}
			switch(Value){
			case 0:
				menuPrintLines(vibratoStr, delayStr, NULL);
				if(enter)
					menuEnter(MENU_VIBRATO_DELAY, VIBRATO, effect_vibrato_get_delay(&SVibrato));

				break;

			case 1:
				menuPrintLines(vibratoStr, frequencyStr, NULL);
				if(enter)
					menuEnter(MENU_VIBRATO_FREQUENCY, VIBRATO, effect_vibrato_get_frequency(&SVibrato));
				break;

			case 2:
				menuPrintLines(vibratoStr, gainStr, NULL);
				if(enter)
					menuEnter(MENU_VIBRATO_GAIN, VIBRATO, 10 * effect_vibrato_get_gain(&SVibrato));
				break;

			case 3:
				menuPrintLines(vibratoStr, backStr, NULL);
				if(enter)
					menuEnter(MENU_HOME, NONE, 1);
				break;
			}
			break;

			case MENU_VIBRATO_DELAY:
				effect_vibrato_set_delay(&SVibrato, Value);
				sprintf(ValueStr, "%lu", Value);
				menuPrintLines(delayStr, ValueStr, msStr);
				if(enter)
					menuEnter(MENU_VIBRATO, VIBRATO, 0);
				break;

			case MENU_VIBRATO_FREQUENCY:
				effect_vibrato_set_frequency(&SVibrato, Value);
				sprintf(ValueStr, "%lu", Value);
				menuPrintLines(frequencyStr, ValueStr, hzStr);
				if(enter)
					menuEnter(MENU_VIBRATO, VIBRATO, 1);
				break;

			case MENU_VIBRATO_GAIN:
				effect_vibrato_set_gain(&SVibrato, ((float32_t)Value)/10.0);
				sprintf(ValueStr, "%lu", Value);
				menuPrintLines(gainStr, ValueStr, NULL);
				if(enter)
					menuEnter(MENU_VIBRATO, VIBRATO, 2);
				break;






			case MENU_FLANGER:

				if(Value > 3){
					Value = 3;
				}
				switch(Value){
				case 0:
					menuPrintLines(flangerStr, delayStr, NULL);
					if(enter)
						menuEnter(MENU_FLANGER_DELAY, FLANGER, effect_flanger_get_delay(&SFlanger));

					break;

				case 1:
					menuPrintLines(flangerStr, frequencyStr, NULL);
					if(enter)
						menuEnter(MENU_FLANGER_FREQUENCY, FLANGER, effect_flanger_get_frequency(&SFlanger));
					break;

				case 2:
					menuPrintLines(flangerStr, gainStr, NULL);
					if(enter)
						menuEnter(MENU_FLANGER_GAIN, FLANGER, 10 * effect_flanger_get_gain(&SFlanger));
					break;

				case 3:
					menuPrintLines(flangerStr, backStr, NULL);
					if(enter)
						menuEnter(MENU_HOME, NONE, 2);
					break;
				}
				break;

				case MENU_FLANGER_DELAY:
					effect_flanger_set_delay(&SFlanger, Value);
					sprintf(ValueStr, "%lu", Value);
					menuPrintLines(delayStr, ValueStr, msStr);
					if(enter)
						menuEnter(MENU_FLANGER, FLANGER, 0);
					break;

				case MENU_FLANGER_FREQUENCY:
					effect_flanger_set_frequency(&SFlanger, Value);
					sprintf(ValueStr, "%lu", Value);
					menuPrintLines(frequencyStr, ValueStr, hzStr);
					if(enter)
						menuEnter(MENU_FLANGER, FLANGER, 1);
					break;

				case MENU_FLANGER_GAIN:
					effect_flanger_set_gain(&SFlanger, ((float32_t)Value)/10.0);
					sprintf(ValueStr, "%lu", Value);
					menuPrintLines(gainStr, ValueStr, NULL);
					if(enter)
						menuEnter(MENU_FLANGER, FLANGER, 2);
					break;





				case MENU_TREMOLO:

					if(Value > 2){
						Value = 2;
					}
					switch(Value){
					case 0:
						menuPrintLines(tremoloStr, frequencyStr, NULL);
						if(enter)
							menuEnter(MENU_TREMOLO_FREQUENCY, TREMOLO, effect_tremolo_get_frequency(&STremolo));
						break;

					case 1:
						menuPrintLines(tremoloStr, gainStr, NULL);
						if(enter)
							menuEnter(MENU_TREMOLO_GAIN, TREMOLO, 10 * effect_tremolo_get_gain(&STremolo));
						break;

					case 2:
						menuPrintLines(tremoloStr, backStr, NULL);
						if(enter)
							menuEnter(MENU_HOME, NONE, 3);
						break;
					}
					break;

					case MENU_TREMOLO_FREQUENCY:
						effect_tremolo_set_frequency(&STremolo, Value);
						sprintf(ValueStr, "%lu", Value);
						menuPrintLines(frequencyStr, ValueStr, hzStr);
						if(enter)
							menuEnter(MENU_TREMOLO, TREMOLO, 0);
						break;

					case MENU_TREMOLO_GAIN:
						effect_tremolo_set_gain(&STremolo, ((float32_t)Value)/10.0);
						sprintf(ValueStr, "%lu", Value);
						menuPrintLines(gainStr, ValueStr, NULL);
						if(enter)
							menuEnter(MENU_TREMOLO, TREMOLO, 1);
						break;


						case MENU_OVERDRIVE:

							if(Value > 0){
								Value = 0;
							}
							switch(Value){
							case 0:
								menuPrintLines(overdriveStr, backStr, NULL);
								if(enter)
									menuEnter(MENU_HOME, NONE, 5);
								break;
							}
							break;

	}
}


