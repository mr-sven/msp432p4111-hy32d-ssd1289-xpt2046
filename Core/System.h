/*
 * System.h
 *
 *  Created on: 11.05.2018
 *      Author: Sven
 */

#ifndef CORE_SYSTEM_H_
#define CORE_SYSTEM_H_

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <Board.h>

class System_
{
public:
	static void SetupCS(void);
	static void SetupSystickTimer(void);
	static uint32_t Millis(void);
	static void StringToHex(char * dest, uint8_t * data, int length);
	static void DelayMs(const unsigned msec);
};

extern System_ System;

#endif /* CORE_SYSTEM_H_ */
