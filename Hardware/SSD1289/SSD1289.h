// Copyright 2019 by Sven Fabricius, see LICENSE.txt
/**
 * @file SSD1289.h
 *
 * @brief SSD1289 accessing functionalities
 *
 * @author Sven Fabricius
 * Contact: Sven.Fabricius@livediesel.de
 *
 */

#ifndef HARDWARE_SSD1289_SSD1289_H_
#define HARDWARE_SSD1289_SSD1289_H_

// DriverLib Includes
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <Hardware/Display.h>

/**
 * configuration struct for SSD1289 Display
 *
 * it is used as pointer to the ASM functions for better access the elements
 */
typedef struct _SSD1289_Config
{
	volatile uint16_t * dataOut;	//!< pointer to the Data Out Port
	volatile uint16_t * dataIn;		//!< pointer to the Data In Port
	volatile uint16_t * dataDir;	//!< pointer to the Data DIR Port
	volatile uint16_t * ctrlOut;	//!< pointer to the Control Out Port
	uint16_t csPin;	//!< bitmask for the CS Pin at Control Port
	uint16_t rsPin;	//!< bitmask for the RS Pin at Control Port
	uint16_t rdPin;	//!< bitmask for the RD Pin at Control Port
	uint16_t wrPin;	//!< bitmask for the WR Pin at Control Port
} SSD1289_Config;

/**
 * interfacing class for accessing SSD1289 Display
 */
class SSD1289: public Display {
private:
	/// configuration container
	SSD1289_Config config;

public:
	/**
	 * Constructor
	 *
	 * @param dataPort the Data Port
	 * @param ctrlPort the Control Port
	 * @param csPin bitmask for the CS Pin at Control Port
	 * @param rsPin bitmask for the RS Pin at Control Port
	 * @param rdPin bitmask for the RD Pin at Control Port
	 * @param wrPin bitmask for the WR Pin at Control Port
	 */
	SSD1289(DIO_PORT_Interruptable_Type * dataPort, DIO_PORT_Interruptable_Type * ctrlPort, uint16_t csPin, uint16_t rsPin, uint16_t rdPin, uint16_t wrPin);

	/**
	 * Fill the area specified by setBounds with color
	 *
	 * @param color the color
	 * @param count32 number of pixels
	 */
	void fill(uint16_t color, uint32_t count32);

public: // IDisplay
	/**
	 * @copydoc Display::init(orientation, width, height)
	 */
	void init(DisplayOrientation orientation, uint32_t width, uint32_t height);

	/**
	 * @copydoc Display::rect(x, y, width, height, color)
	 */
    void rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color);

	/**
	 * @copydoc Display::pixel(x, y, color)
	 */
    void pixel(uint32_t x, uint32_t y, uint16_t color);

	/**
	 * @copydoc Display::setBounds(x, y, width, height)
	 */
	void setBounds(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	/**
	 * @copydoc Display::blit16(data, count)
	 */
    void blit16(const uint16_t* data, uint32_t count);

	/**
	 * @copydoc Display::drawChar(xx, yy, c, color)
	 */
    uint32_t drawChar(uint32_t xx, uint32_t yy, char c, uint16_t color);
};

#endif /* HARDWARE_SSD1289_SSD1289_H_ */
