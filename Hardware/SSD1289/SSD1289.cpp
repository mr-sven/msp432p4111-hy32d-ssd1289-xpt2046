/*
 * SSD1289.cpp
 *
 *  Created on: 26.12.2018
 *      Author: Sven
 */

#include <Hardware/SSD1289/SSD1289.h>

#include <Hardware/SSD1289/SSD1289_cmd.h>

#include <Core/System.h>

static const uint16_t ssd1289_init [] = {
	SSD1289_OSC_START, SSD1289_OSC_START_OSCEN,		// Enable LCD Oscillator
	_DELAY_MS, 15,									// wait 15 ms
	SSD1289_PWR_CTRL_1, 0xAAAC,	// 1010 100 0 1010 010 0  // fosc / 4
	SSD1289_PWR_CTRL_2, 0x0002,	// VCIX2 5.1V
	SSD1289_PWR_CTRL_3, 0x000A,	// 000010000000 1100	// Vref x 2.500V
	SSD1289_PWR_CTRL_4, 0x2C00,	// 00 1 01011 00000000	// VcomG = 1 //
	SSD1289_PWR_CTRL_5, 0x00B8,
	_DELAY_MS, 15,
	SSD1289_DRV_OUT_CTRL, 0x2B3F,	/* 320*240 0x2B3F */
	SSD1289_LCD_DRV_AC_CTRL, 0x0600,
	SSD1289_SLEEP_MODE, 0x0000,
	SSD1289_ENTRY_MODE, 0x6030,
	_DELAY_MS, 20,
	SSD1289_COMP_REG_1, 0x0000,
	SSD1289_COMP_REG_2, 0x0000,
	SSD1289_HORIZ_PORCH, 0xEF1C,
	SSD1289_VERT_PORCH, 0x0003,
	SSD1289_DISP_CTRL, 0x0233,
	SSD1289_FRM_CYL_CTRL, 0x5312,
	SSD1289_GAT_SCN_STRT_POS, 0x0000,
	_DELAY_MS, 20,
	SSD1289_VERT_SCROLL_1, 0x0000,
	SSD1289_VERT_SCROLL_2, 0x0000,
	SSD1289_FIRST_WND_START, 0x0000,
	SSD1289_FIRST_WND_END, 0x013F,
	SSD1289_HORIZ_RAM_ADDR_POS, 0xEF00,
	SSD1289_VERT_RAM_ADDR_STRT_POS, 0x0000,
	SSD1289_VERT_RAM_ADDR_END_POS, 0x013F,
	_DELAY_MS, 50,
	SSD1289_GAMMA_CTRL_1, 0x0707,
	SSD1289_GAMMA_CTRL_2, 0x0704,
	SSD1289_GAMMA_CTRL_3, 0x0204,
	SSD1289_GAMMA_CTRL_4, 0x0201,
	SSD1289_GAMMA_CTRL_5, 0x0303,
	SSD1289_GAMMA_CTRL_6, 0x0204,
	SSD1289_GAMMA_CTRL_7, 0x0204,
	SSD1289_GAMMA_CTRL_8, 0x0502,
	SSD1289_GAMMA_CTRL_9, 0x0302,
	SSD1289_GAMMA_CTRL_10, 0x0500,
	SSD1289_RAM_WR_MASK_1, 0x0000,
	SSD1289_RAM_WR_MASK_2, 0x0000,
	SSD1289_SET_GDDRAM_Y_ADDR_CNT, 0,
	SSD1289_SET_GDDRAM_X_ADDR_CNT, 0,
	_DELAY_MS, 50,
	_END
};


extern "C"
{
extern void SSD1289_WriteReg(SSD1289_Config * config, uint16_t command, uint16_t data);
extern void SSD1289_Fill(SSD1289_Config * config, uint16_t color, uint32_t count32);
}
/*
void SSD1289_WriteReg(volatile uint16_t * dataOut, volatile uint16_t * ctlOut, uint16_t rsPin, uint16_t wrPin, uint16_t command, uint16_t data)
{
	*ctlOut &= ~(rsPin);
	*dataOut = command;
	*ctlOut &= ~(wrPin);
	*ctlOut |= wrPin;
	*ctlOut |= rsPin;

	*dataOut = data;
	*ctlOut &= ~(wrPin);
	*ctlOut |= wrPin;
}
*/

SSD1289::SSD1289(DIO_PORT_Interruptable_Type * dataPort, DIO_PORT_Interruptable_Type * ctrlPort, uint16_t csPin, uint16_t rsPin, uint16_t rdPin, uint16_t wrPin)
{
	// store pins
	config.csPin = csPin;
	config.rsPin = rsPin;
	config.rdPin = rdPin;
	config.wrPin = wrPin;

	// init control pins
	ctrlPort->DIR |= (csPin | rsPin | rdPin | wrPin);
	ctrlPort->OUT |= (csPin | rsPin | rdPin | wrPin);

	// load data port registers
	config.dataOut = &dataPort->OUT;
	config.dataIn = &dataPort->IN;
	config.dataDir = &dataPort->DIR;

	// load control port register
	config.ctrlOut = &ctrlPort->OUT;
}

void SSD1289::Init(void)
{
	// set data direction
	*config.dataDir = 0xffff;

	// select display
	*config.ctrlOut &= ~(config.csPin);

	// get init data start pointer
	const uint16_t *addr = ssd1289_init;

	while (1)
	{
		uint16_t command = *addr++;

		// check if command is end
		if (command == _END)
		{
			break;
		}
		// execute delay
		else if(command == _DELAY_MS)
		{
			System.DelayMs(*addr++);
		}
		else
		{
			SSD1289_WriteReg(&config, command, *addr++);
		}
	}

}

void SSD1289::Fill(uint16_t color, uint32_t count32)
{
	SSD1289_Fill(&config, color, count32);
}

void SSD1289::SetBounds(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	SSD1289_WriteReg(&config, SSD1289_HORIZ_RAM_ADDR_POS, (((x + width - 1) << 8) | x));
	SSD1289_WriteReg(&config, SSD1289_VERT_RAM_ADDR_STRT_POS, y);
	SSD1289_WriteReg(&config, SSD1289_VERT_RAM_ADDR_END_POS, (y + height - 1));

	SSD1289_WriteReg(&config, SSD1289_SET_GDDRAM_X_ADDR_CNT, x);
	SSD1289_WriteReg(&config, SSD1289_SET_GDDRAM_Y_ADDR_CNT, y);
}


void SSD1289::Rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color)
{

}

void SSD1289::Pixel(uint32_t x, uint32_t y, uint16_t color)
{

}

void SSD1289::Blit16(const uint16_t* d, uint32_t count, bool cont)
{

}

