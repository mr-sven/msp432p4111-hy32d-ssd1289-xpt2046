/*
 * Display.h
 *
 *  Created on: 04.11.2018
 *      Author: Sven
 */

#ifndef HARDWARE_DISPLAY_H_
#define HARDWARE_DISPLAY_H_

#include <stdint.h>

enum TextAlign
{
	TextAlignLeft,
	TextAlignCenter,
	TextAlignRight
};

enum DisplayOrientation
{
	DisplayOrientation_0,
	DisplayOrientation_90,
	DisplayOrientation_180,
	DisplayOrientation_270
};

class Display
{
private:
	uint8_t fontHeight;
	uint16_t fontStart;
	const uint8_t* fontData;
	DisplayOrientation displayOrientation;
	uint32_t displayWidth;
	uint32_t displayHeight;

	uint8_t measureChar(char c);
public:
	void SetFont(const uint8_t* font);

	uint32_t DrawChar(uint32_t xx, uint32_t yy, char c, uint16_t color);

	uint32_t DrawString(const char* s, uint32_t len, uint32_t x, uint32_t y, uint16_t color);
	uint32_t DrawString(const char* s, uint32_t x, uint32_t y, uint16_t color);

	uint32_t MeasureString(const char* s, uint32_t len);
	uint32_t MeasureString(const char* s);

	void Label(const char* s, TextAlign align, uint32_t x, uint32_t y, uint32_t width, uint16_t fgColor, uint16_t bgColor);

	void Draw4(uint32_t cx, uint32_t cy, uint32_t x, uint32_t y, uint16_t color);
	void Draw8(uint32_t cx, uint32_t cy, uint32_t x, uint32_t y, uint16_t color, bool fill);

	void Circle(uint32_t cx, uint32_t cy, uint32_t radius, uint16_t color, bool fill);
	void Line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);
	void Triangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint16_t color, bool fill);

	void SetOrientationSize(DisplayOrientation orientation, uint32_t width, uint32_t height);

public: // abstract function
    virtual ~Display() {}
    virtual void Rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color) = 0;
	virtual void Pixel(uint32_t x, uint32_t y, uint16_t color) = 0;
	virtual void SetBounds(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
	virtual void Blit16(const uint16_t* d, uint32_t count, bool cont) = 0;
};

#endif /* HARDWARE_DISPLAY_H_ */
