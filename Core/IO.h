/*
 * IO.h
 *
 *  Created on: 11.05.2018
 *      Author: Sven
 */

#ifndef CORE_IO_H_
#define CORE_IO_H_

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <Board.h>

class IO_
{
public:
	static void SetupOutputs(void);
};

extern IO_ IO;

#endif /* CORE_IO_H_ */
