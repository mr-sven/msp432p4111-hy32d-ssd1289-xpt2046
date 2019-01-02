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

	SSD1289_PWR_CTRL_1, SSD1289_PWR_CTRL_1_DCT(10)|	// fosc / 4
						SSD1289_PWR_CTRL_1_BT(5)|	// VGH: VCI x 5; VGL: -(VGH) + VCI; VGH-boost: +5 ; VGL-boost:-4
						SSD1289_PWR_CTRL_1_DC(10)|	// fosc / 4
						SSD1289_PWR_CTRL_1_AP(6),	// Small to medium

	SSD1289_PWR_CTRL_2, SSD1289_PWR_CTRL_2_VRC(2),	// VCIX2: 5.3V

	SSD1289_PWR_CTRL_3, SSD1289_PWR_CTRL_3_VRH(10),	// VLCD63: Vref x 2.335

	SSD1289_PWR_CTRL_4, SSD1289_PWR_CTRL_4_VCOMG|	// VcomG
						SSD1289_PWR_CTRL_4_VDV(12),	// Vcom: VLCD63 x 0.96

	SSD1289_PWR_CTRL_5, SSD1289_PWR_CTRL_5_NOTP|	// nOTP
						SSD1289_PWR_CTRL_5_VCM(56), // VcomH: VLCD63 x 0.91

	_DELAY_MS, 15,									// wait 15 ms

	SSD1289_DRV_OUT_CTRL, SSD1289_DRV_OUT_CTRL_REV| // REV
						  SSD1289_DRV_OUT_CTRL_BGR| // BGR
						  SSD1289_DRV_OUT_CTRL_TB|  // TB
						  SSD1289_DRV_OUT_CTRL_MUX(319),	// Max LCD Line

	SSD1289_LCD_DRV_AC_CTRL, SSD1289_LCD_DRV_AC_CTRL_BC|	// BC
							 SSD1289_LCD_DRV_AC_CTRL_EOR|	// EOR
							 SSD1289_LCD_DRV_AC_CTRL_NW(0),	// NW 0

	SSD1289_SLEEP_MODE, 0,							// exit sleep mode

	SSD1289_ENTRY_MODE, SSD1289_ENTRY_MODE_DFM(3)|	// 65k color (POR)
						SSD1289_ENTRY_MODE_ID(3),	// Horizontal: increment; Vertical: increment

	_DELAY_MS, 20,									// wait 20 ms

	SSD1289_COMP_REG_1, 0,
	SSD1289_COMP_REG_2, 0,

	SSD1289_HORIZ_PORCH, SSD1289_HORIZ_PORCH_XL(239)|	// valid pixel per line 239 + 1
						 SSD1289_HORIZ_PORCH_HBP(28),	// No. of clock cycle of DOTCLK: 30

	SSD1289_VERT_PORCH, SSD1289_VERT_PORCH_VFP(0)|	// No. of clock cycle of HSYNC: 1
						SSD1289_VERT_PORCH_VBP(3),	// No. of clock cycle of HSYNC: 4

	SSD1289_DISP_CTRL, SSD1289_DISP_CTRL_VLE(1)|
					   SSD1289_DISP_CTRL_GON|
					   SSD1289_DISP_CTRL_DTE|
					   SSD1289_DISP_CTRL_D(3),

	SSD1289_FRM_CYL_CTRL, SSD1289_FRM_CYL_CTRL_NO(1)|	// 1 clock cycle (POR) of non-overlap
						  SSD1289_FRM_CYL_CTRL_SDT(1)|	// 1 clock cycle (POR) Delay amount of the source output
						  SSD1289_FRM_CYL_CTRL_EQ(3)|	// 4 clock cycle equalizing period
						  SSD1289_FRM_CYL_CTRL_SRTN|	// RTN3-0 value will be count
						  SSD1289_FRM_CYL_CTRL_RTN(2),	// no. of clocks in each line

	SSD1289_GAT_SCN_STRT_POS, SSD1289_GAT_SCN_STRT_POS_SCN(0),	// scanning starting position

	_DELAY_MS, 20,									// wait 20 ms

	SSD1289_FIRST_WND_START, SSD1289_FIRST_WND_START_SS(0),
	SSD1289_FIRST_WND_END, SSD1289_FIRST_WND_END_SE(319),

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

	_DELAY_MS, 50,									// wait 50 ms

	_END
};


extern "C"
{
extern void SSD1289_WriteReg(SSD1289_Config * config, uint16_t command, uint16_t data);
extern void SSD1289_Fill(SSD1289_Config * config, uint16_t color, uint32_t count32);
}

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

void SSD1289::init(DisplayOrientation orientation, uint32_t width, uint32_t height)
{
	// call parent
	Display::init(orientation, width, height);

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
		if(command == _DELAY_MS)
		{
			System.delayMs(*addr++);
			continue;
		}

		// write register default
		SSD1289_WriteReg(&config, command, *addr++);
	}

	uint16_t entryModeReg = SSD1289_ENTRY_MODE_DFM(3) | SSD1289_ENTRY_MODE_ID(3);
	uint16_t driverOutCtrlReg = SSD1289_DRV_OUT_CTRL_REV | SSD1289_DRV_OUT_CTRL_BGR | SSD1289_DRV_OUT_CTRL_MUX(319);

	switch (displayOrientation)
	{
	case DisplayOrientation::Portrait:
		driverOutCtrlReg |= SSD1289_DRV_OUT_CTRL_TB;
		break;
	case DisplayOrientation::Landscape_90:
		driverOutCtrlReg |= SSD1289_DRV_OUT_CTRL_TB | SSD1289_DRV_OUT_CTRL_RL;
		entryModeReg |= SSD1289_ENTRY_MODE_AM;
		break;
	case DisplayOrientation::Portrait_180:
		driverOutCtrlReg |= SSD1289_DRV_OUT_CTRL_RL;
		break;
	case DisplayOrientation::Landscape_270:
		entryModeReg |= SSD1289_ENTRY_MODE_AM;
		break;
	}

	SSD1289_WriteReg(&config, SSD1289_DRV_OUT_CTRL, driverOutCtrlReg);
	SSD1289_WriteReg(&config, SSD1289_ENTRY_MODE, entryModeReg);
}

void SSD1289::fill(uint16_t color, uint32_t count32)
{
	SSD1289_Fill(&config, color, count32);
}

void SSD1289::setBounds(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	// swap values on landscape
	if (displayOrientation == DisplayOrientation::Landscape_90 || displayOrientation == DisplayOrientation::Landscape_270)
	{
		uint32_t tmp = x;
		x = y;
		y = tmp;

		tmp = height;
		height = width;
		width = tmp;
	}

	SSD1289_WriteReg(&config, SSD1289_HORIZ_RAM_ADDR_POS, SSD1289_HORIZ_RAM_ADDR_POS_HSA(x) | SSD1289_HORIZ_RAM_ADDR_POS_HSE(x + width - 1));
	SSD1289_WriteReg(&config, SSD1289_VERT_RAM_ADDR, SSD1289_VERT_RAM_ADDR_VA(y)); // start
	SSD1289_WriteReg(&config, SSD1289_VERT_RAM_ADDR + 1, SSD1289_VERT_RAM_ADDR_VA(y + height - 1)); // end

	SSD1289_WriteReg(&config, SSD1289_SET_GDDRAM_X_ADDR_CNT, x);
	SSD1289_WriteReg(&config, SSD1289_SET_GDDRAM_Y_ADDR_CNT, y);
}


void SSD1289::rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color)
{

}

void SSD1289::pixel(uint32_t x, uint32_t y, uint16_t color)
{

}

void SSD1289::blit16(const uint16_t* data, uint32_t count, bool contData)
{

}

