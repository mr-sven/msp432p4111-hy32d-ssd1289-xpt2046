/*
 * System.cpp
 *
 *  Created on: 11.05.2018
 *      Author: Sven
 */

#include <core/System.h>

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <Board.h>

void System_::setupCS(void)
{

    // Before we start we have to change VCORE to 1 to support the 48MHz frequency
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    MAP_FlashCtl_A_setWaitState(FLASH_A_BANK0, 3);
    MAP_FlashCtl_A_setWaitState(FLASH_A_BANK1, 3);

    // Initializes Clock System
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
}

void System_::setupSystickTimer(void)
{
    // Configuring SysTick to trigger at 1 ms
    MAP_SysTick_enableModule();
    MAP_SysTick_setPeriod(CS_48MHZ / 1000 - 1);
    MAP_SysTick_enableInterrupt();
}

volatile uint32_t SysTickCntMsec = 0;
extern "C" void SysTick_Handler(void)
{
	SysTickCntMsec++;
}

uint32_t System_::millis(void)
{
	return SysTickCntMsec;
}

uint32_t millis(void)
{
	return SysTickCntMsec;
}

const char hex[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
void System_::stringToHex(char * dest, uint8_t * data, int length)
{
	while (length > 0)
	{
		length--;
		*dest++ = hex[*data >> 4];
		*dest++ = hex[*data++ & 0x0f];
	}
	*dest = 0;
}


void System_::delayMs(const unsigned msec)
{
    const unsigned wu = SysTickCntMsec + msec;

    do {
#if defined ( __TMS470__ )
        __wfi();
#else
        __sleep();
#endif
    } while (SysTickCntMsec < wu);
}

System_ System;
