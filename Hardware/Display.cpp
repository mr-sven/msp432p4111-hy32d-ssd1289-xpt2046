/*
 * Display.cpp
 *
 *  Created on: 04.11.2018
 *      Author: Sven
 */


#include <Hardware/Display.h>

#include <string.h>

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

void Display::setFont(const uint8_t* font)
{
	fontData = font;
	fontStart = FONT_START;
	fontHeight = FONT_HEIGHT;
}

uint32_t Display::drawChar(uint32_t xx, uint32_t yy, char c, uint16_t color)
{
	if (c == 32)
	{
		return fontHeight >> 2;  // Space is 1/4 font height (yuk);
	}

	uint8_t width = FONT_CHAR_WIDTH(c);
	uint32_t offset = FONT_CHAR_OFFSET(c);

	for (int y = 0; y < fontHeight; y++)
	{
		uint8_t p = 0;
		for (uint8_t x = 0; x < width; x++)
		{
			if (x % 8 == 0)
			{
				p = FNTB(offset++);
			}
			if(p & 0x01)
			{
				//ILI9341.Rectangle(x + xx, y + yy, 1, 1, color);
				pixel(x + xx, y + yy, color);
			}
			p >>= 1;
		}
	}
	return width;
}

uint32_t Display::drawString(const char* s, uint32_t len, uint32_t x, uint32_t y, uint16_t color)
{
	for (uint16_t i = 0; i < len; i++)
	{
		x += drawChar(x, y, s[i], color) + 1;
	}
	return x;
}

uint32_t Display::drawString(const char* s, uint32_t x, uint32_t y, uint16_t color)
{
	return drawString(s, strlen(s), x, y, color);
}

void Display::label(const char* s, TextAlign align, uint32_t x, uint32_t y, uint32_t width, uint16_t fgColor, uint16_t bgColor)
{
	uint16_t hLineBuf[512];
	uint32_t length = strlen(s);
	uint32_t strWidth = measureString(s, length);
	uint32_t start = 0;
	uint32_t space = width - strWidth;

	switch (align)
	{
	case TextAlign::Left:
		start = x;
		rect(x + strWidth, y, space, fontHeight, bgColor);
		break;
	case TextAlign::Right:
		start = x + space;
		rect(x, y, space, fontHeight, bgColor);
		break;
	case TextAlign::Center:
		start = x + space / 2;
		rect(x, y, space / 2, fontHeight, bgColor);
		rect(start + strWidth, y, space / 2, fontHeight, bgColor);
		break;
	}

	while (length--)
	{
		char c = *s++;
		if (c == 32)
		{
			rect(start, y, fontHeight >> 2, fontHeight, bgColor);
			start += fontHeight >> 2;  // Space is 1/4 font height (yuk);
			continue;
		}

		uint32_t cOffset = FONT_CHAR_OFFSET(c);
		uint8_t cWidth = FONT_CHAR_WIDTH(c);
		uint16_t * hLineBufP = hLineBuf;

		//hLineBufP += (fontHeight * cWidth) - cWidth; // move data pointer to start of last line

		for (int cY = 0; cY < fontHeight; cY++)
		{
			uint8_t p = 0;
			for (int cX = 0; cX < cWidth; cX++)
			{
				if (cX % 8 == 0)
				{
					p = FNTB(cOffset++);
				}

				*hLineBufP++ = (p & 0x01) ? fgColor : bgColor;

				p >>= 1;
			}
			//hLineBufP -= cWidth + cWidth; // move data pointer to start of previous line
		}

		setBounds(start, y, cWidth, fontHeight);
		blit16(hLineBuf, fontHeight * cWidth);
		rect(start + cWidth, y, 1, fontHeight, bgColor);

		start += cWidth + 1;
	}
}

uint8_t Display::measureChar(char c)
{
	if (c == 32)
	{
		return fontHeight >> 2;  // Space is 1/4 font height (yuk);
	}

	return FONT_CHAR_WIDTH(c);
}

uint32_t Display::measureString(const char* s, uint32_t len)
{
	uint32_t x = 0;
	while (len--)
	{
		x += measureChar(*s++) + 1;
	}
	return x;
}

uint32_t Display::measureString(const char* s)
{
	return measureString(s, strlen(s));
}

void Display::draw4(uint32_t cx, uint32_t cy, uint32_t x, uint32_t y, uint16_t color)
{
	pixel(cx + x, cy + y, color);
	if (x != 0) pixel(cx - x, cy + y, color);
	if (y != 0) pixel(cx + x, cy - y, color);
	pixel(cx - x, cy - y, color);
}

void Display::draw8(uint32_t cx, uint32_t cy, uint32_t x, uint32_t y, uint16_t color, bool fill)
{
	if (fill)
	{
		rect(cx - y, cy - x, y << 1, 1, color);
		rect(cx - x, cy - y, x << 1, 1, color);
		rect(cx - x, cy + y, x << 1, 1, color);
		rect(cx - y, cy + x, y << 1, 1, color);
	}
	else
	{
		draw4(cx, cy, x, y, color);
		if (x != y)
		{
			draw4(cx, cy, y, x, color);
		}
	}
}

void Display::circle(uint32_t cx, uint32_t cy, uint32_t radius, uint16_t color, bool fill)
{
	int error = -radius;
	uint32_t x = radius;
	uint32_t y = 0;

//	draws in 4 strips from top down and middle up + reflection about y
	while (x >= y)
	{
		draw8(cx, cy, x, y, color, fill);
		error += y;
		++y;
		error += y;
		if (error >= 0)
		{
			--x;
			error -= x<<1;
		}
	}
}

void Display::line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color)
{
    if(x0 == x1) // Vertical Line
    {
        if(y0 > y1) _swap_uint32_t(y0, y1);
        rect(x0, y0, 1, y1 - y0 + 1, color);
        return;
    }
    else if(y0 == y1) // Horizontal Line
    {
        if(x0 > x1) _swap_uint32_t(x0, x1);
        rect(x0, y0, x1 - x0 + 1, 1, color);
        return;
    }

	uint32_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
    	_swap_uint32_t(x0, y0);
    	_swap_uint32_t(x1, y1);
    }

    if (x0 > x1)
    {
    	_swap_uint32_t(x0, x1);
    	_swap_uint32_t(y0, y1);
    }

    uint32_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int err = dx / 2;
    int ystep;

    if (y0 < y1)
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    for (; x0<=x1; x0++)
    {
        if (steep)
        {
            pixel(y0, x0, color);
        }
        else
        {
        	pixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

void Display::triangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint16_t color, bool fill)
{
	if (!fill)
	{
		line(x0, y0, x1, y1, color);
	    line(x1, y1, x2, y2, color);
	    line(x2, y2, x0, y0, color);
	    return;
	}

	int a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1)
	{
		_swap_uint32_t(y0, y1);
		_swap_uint32_t(x0, x1);
	}
	if (y1 > y2)
	{
		_swap_uint32_t(y2, y1);
		_swap_uint32_t(x2, x1);
	}
	if (y0 > y1)
	{
		_swap_uint32_t(y0, y1);
		_swap_uint32_t(x0, x1);
	}


	if(y0 == y2)
	{ // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if(x1 < a)      a = x1;
		else if(x1 > b) b = x1;
		if(x2 < a)      a = x2;
		else if(x2 > b) b = x2;
		rect(a, y0, b-a+1, 1, color);
		return;
	}

	int
	dx01 = x1 - x0,
	dy01 = y1 - y0,
	dx02 = x2 - x0,
	dy02 = y2 - y0,
	dx12 = x2 - x1,
	dy12 = y2 - y1;
	int
	sa   = 0,
	sb   = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if(y1 == y2) last = y1;   // Include y1 scanline
	else         last = y1-1; // Skip it

	for(y=y0; y<=last; y++)
	{
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if(a > b) _swap_uint32_t(a,b);
		rect(a, y, b-a+1, 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++)
	{
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if(a > b) _swap_uint32_t(a,b);
		rect(a, y, b-a+1, 1, color);
	}
}


void Display::init(DisplayOrientation orientation, uint32_t width, uint32_t height)
{
	displayOrientation = orientation;
	displayWidth = width;
	displayHeight = height;
}
