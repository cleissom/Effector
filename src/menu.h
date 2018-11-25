/*
 * menu.h
 *
 *  Created on: Nov 24, 2018
 *      Author: Cleissom
 */

#ifndef MENU_H_
#define MENU_H_

typedef enum {
	NONE,
	ECHO,
	VIBRATO
} filter_type_enum;

typedef enum {
	ATTR_NONE,
	ATTR_DELAY,
	ATTR_GAIN,
	ATTR_FREQUENCY
} filter_attribute_enum;

typedef enum {
	MENU_HOME,
	MENU_ECHO,
	MENU_ECHO_DELAY,
	MENU_ECHO_GAIN,
	MENU_VIBRATO,
	MENU_VIBRATO_DELAY,
	MENU_VIBRATO_FREQUENCY,
	MENU_VIBRATO_GAIN
} menu_state_enum;



#endif /* MENU_H_ */
