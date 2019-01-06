// Copyright 2019 by Sven Fabricius, see LICENSE.txt
/**
 * @file Display.h
 *
 * @brief Display accessing functionalities
 *
 * @author Sven Fabricius
 * Contact: Sven.Fabricius@livediesel.de
 *
 */

#ifndef HARDWARE_DISPLAY_H_
#define HARDWARE_DISPLAY_H_

#include <stdint.h>

/**
 * Swap UINT32 values between two registers
 */
#ifndef _swap_uint32_t
#define _swap_uint32_t(a, b) { uint32_t t = a; a = b; b = t; }
#endif

/// short notation for accessing font data byte
#define FNTB(_x) fontData[_x]

/// short notation for accessing font data short
#define FNTW(_x) ((fontData[_x + 1] << 8) | fontData[_x])

/// offset in font data for start char code
#define FONT_START_OFFSET			2

/// offset in font data for fort height
#define FONT_HEIGHT_OFFSET			6

/// size of the font data header block
#define FONT_HEADER_SIZE			8

/// size of the font char width and offset block
#define FONT_CHAR_WIDTH_OFF_SIZE	4

/// short notation for access start char code
#define FONT_START					FNTW(FONT_START_OFFSET)

/// short notation for access font height
#define FONT_HEIGHT					FNTB(FONT_HEIGHT_OFFSET)

/// short notation for access char width
#define FONT_CHAR_WIDTH(c)			FNTB(FONT_HEADER_SIZE + (c - fontStart) * FONT_CHAR_WIDTH_OFF_SIZE)

/// short notation for access char data offset
#define FONT_CHAR_OFFSET(c)			FNTW(FONT_HEADER_SIZE + (c - fontStart) * FONT_CHAR_WIDTH_OFF_SIZE + 1)


/**
 * Text align enum for positioning text in a label
 */
enum class TextAlign
{
	Left,	//!< text align left
	Center,	//!< text align center
	Right	//!< text align right
};

/**
 * Display orientation setting
 */
enum class DisplayOrientation
{
	Portrait,		//!< default portrait orientation
	Landscape_90,	//!< landscape at 90°
	Portrait_180,	//!< portrait at 180°
	Landscape_270	//!< landscape at 270°
};

/**
 * Abstract class for accessing Display functions
 */
class Display
{
protected:
	/// font height for later processing
	uint8_t fontHeight;

	/// font start char code for later processing
	uint16_t fontStart;

	/// pointer to the current font data array
	const uint8_t* fontData;

	/// contains the current display orientation
	DisplayOrientation displayOrientation;

	/// width of the display
	uint32_t displayWidth;

	/// height of the display
	uint32_t displayHeight;

	/**
	 * Measures the width of the specified char
	 *
	 * @param c char to measure
	 * @return width of the char
	 */
	uint8_t measureChar(char c);

public:
	/**
	 * sets the font for the next text output
	 *
	 * @param font pointer to the font data
	 */
	void setFont(const uint8_t* font);

	/**
	 * draws a char at the specified position
	 *
	 * This function can be overridden to implement better output for display performance.
	 *
	 * @param xx left start
	 * @param yy top start
	 * @param c char to write
	 * @param color color to output
	 * @return the width of the char
	 */
	virtual uint32_t drawChar(uint32_t xx, uint32_t yy, char c, uint16_t color);

	/**
	 * draws a string at the specified position
	 *
	 * @param s pointer to the text
	 * @param len length of the text to output
	 * @param x left start
	 * @param y top start
	 * @param color color to output
	 * @return right end of the text
	 */
	uint32_t drawString(const char* s, uint32_t len, uint32_t x, uint32_t y, uint16_t color);

	/**
	 * draws a 0 terminated string at the specified position
	 *
	 * @overload
	 */
	uint32_t drawString(const char* s, uint32_t x, uint32_t y, uint16_t color);

	/**
	 * Measures the width of the specified string
	 *
	 * @param s pointer to the text
	 * @param len length of the text to output
	 * @return width of the text
	 */
	uint32_t measureString(const char* s, uint32_t len);

	/**
	 *  Measures the width of the specified 0 terminated string
	 *
	 * @overload
	 */
	uint32_t measureString(const char* s);

	void label(const char* s, TextAlign align, uint32_t x, uint32_t y, uint32_t width, uint16_t fgColor, uint16_t bgColor);

	void draw4(uint32_t cx, uint32_t cy, uint32_t x, uint32_t y, uint16_t color);
	void draw8(uint32_t cx, uint32_t cy, uint32_t x, uint32_t y, uint16_t color, bool fill);

	void circle(uint32_t cx, uint32_t cy, uint32_t radius, uint16_t color, bool fill);
	void line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);
	void triangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint16_t color, bool fill);

	void init(DisplayOrientation orientation, uint32_t width, uint32_t height);

public: // abstract function
    virtual ~Display() {}
    virtual void rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color) = 0;
	virtual void pixel(uint32_t x, uint32_t y, uint16_t color) = 0;
	virtual void setBounds(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
	virtual void blit16(const uint16_t* data, uint32_t count) = 0;
};

#endif /* HARDWARE_DISPLAY_H_ */
