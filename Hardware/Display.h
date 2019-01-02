/*
 * Display.h
 *
 *  Created on: 04.11.2018
 *      Author: Sven
 */

#ifndef HARDWARE_DISPLAY_H_
#define HARDWARE_DISPLAY_H_

#include <stdint.h>

#ifndef _swap_uint32_t
#define _swap_uint32_t(a, b) { uint32_t t = a; a = b; b = t; }
#endif

#define FNTB(_x) fontData[_x]
#define FNTW(_x) ((fontData[_x + 1] << 8) | fontData[_x])

#define FONT_START_OFFSET			2
#define FONT_HEIGHT_OFFSET			6
#define FONT_HEADER_SIZE			8
#define FONT_CHAR_WIDTH_OFF_SIZE	4

#define FONT_START					FNTW(FONT_START_OFFSET)
#define FONT_HEIGHT					FNTB(FONT_HEIGHT_OFFSET)
#define FONT_CHAR_WIDTH(c)			FNTB(FONT_HEADER_SIZE + (c - fontStart) * FONT_CHAR_WIDTH_OFF_SIZE)
#define FONT_CHAR_OFFSET(c)			FNTW(FONT_HEADER_SIZE + (c - fontStart) * FONT_CHAR_WIDTH_OFF_SIZE + 1)

enum class TextAlign
{
	Left,
	Center,
	Right
};

enum class DisplayOrientation
{
	Portrait,
	Landscape_90,
	Portrait_180,
	Landscape_270
};

class Display
{
protected:
	uint8_t fontHeight;
	uint16_t fontStart;
	const uint8_t* fontData;

	uint8_t measureChar(char c);

	DisplayOrientation displayOrientation;
	uint32_t displayWidth;
	uint32_t displayHeight;

public:
	void setFont(const uint8_t* font);

	virtual uint32_t drawChar(uint32_t xx, uint32_t yy, char c, uint16_t color);

	uint32_t drawString(const char* s, uint32_t len, uint32_t x, uint32_t y, uint16_t color);
	uint32_t drawString(const char* s, uint32_t x, uint32_t y, uint16_t color);

	uint32_t measureString(const char* s, uint32_t len);
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
