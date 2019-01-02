/*
 * System.h
 *
 *  Created on: 11.05.2018
 *      Author: Sven
 */

#ifndef CORE_SYSTEM_H_
#define CORE_SYSTEM_H_

#include <stdint.h>

class System_
{
public:
	static void setupCS(void);
	static void setupSystickTimer(void);
	static uint32_t millis(void);
	static void stringToHex(char * dest, uint8_t * data, int length);
	static void delayMs(const unsigned msec);
};

extern System_ System;

#endif /* CORE_SYSTEM_H_ */
