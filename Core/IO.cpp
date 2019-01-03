// Copyright 2019 by Sven Fabricius, see LICENSE.txt
/**
 * @file IO.cpp
 *
 * @brief Basic IO calls
 *
 * @author Sven Fabricius
 * Contact: Sven.Fabricius@livediesel.de
 *
 */

#include <core/IO.h>

// DriverLib Includes
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <Board.h>

void IO_::setupOutputs(void)
{
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PA, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PB, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PC, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PD, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_PE, PIN_ALL16);

    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PA, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PB, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PC, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PD, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PE, PIN_ALL16);

#if !defined(LFXT_ENABLED) || !defined(HFXT_ENABLED) || !defined(JTAG_ENABLED)

    MAP_GPIO_setAsOutputPin(GPIO_PORT_PJ,
#if !defined(LFXT_ENABLED)
    		GPIO_PIN0 || GPIO_PIN1 ||
#endif
#if !defined(HFXT_ENABLED)
    		GPIO_PIN2 || GPIO_PIN3 ||
#endif
#if !defined(JTAG_ENABLED)
    		GPIO_PIN4 || GPIO_PIN5 ||
#endif
			0
    );

    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PJ,
#if !defined(LFXT_ENABLED)
    		GPIO_PIN0 || GPIO_PIN1 ||
#endif
#if !defined(HFXT_ENABLED)
    		GPIO_PIN2 || GPIO_PIN3 ||
#endif
#if !defined(JTAG_ENABLED)
    		GPIO_PIN4 || GPIO_PIN5 ||
#endif
			0
    );

#endif
}

IO_ IO;
