// Copyright 2019 by Sven Fabricius, see LICENSE.txt
/**
 * @file System.h
 *
 * @brief System specific calls
 *
 * @author Sven Fabricius
 * Contact: Sven.Fabricius@livediesel.de
 *
 */

#ifndef CORE_SYSTEM_H_
#define CORE_SYSTEM_H_

#include <stdint.h>

/**
 * Implementation of system specific calls
 *
 */
class System_
{
public:
	/**
	 * Initialize Clock System
	 */
	static void setupCS(void);

	/**
	 * Initialize systick timer for millisecond counter
	 */
	static void setupSystickTimer(void);

	/**
	 * returns millisecond counter
	 */
	static uint32_t millis(void);

	/**
	 * Convert byte array to string
	 *
	 * @param dest the destination for hex out
	 * @param data the data array to convert
	 * @param length the length of the data array
	 */
	static void stringToHex(char * dest, uint8_t * data, int length);

	/**
	 * waits sepcified milliseconds respecting interrupts
	 */
	static void delayMs(const unsigned msec);
};

/**
 * Static class implementation
 */
extern System_ System;

#endif /* CORE_SYSTEM_H_ */
