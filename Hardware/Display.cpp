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

#ifndef _swap_uint32_t
#define _swap_uint32_t(a, b) { uint32_t t = a; a = b; b = t; }
#endif

void Display::SetFont(const uint8_t* font)
{
	fontData = font;
	fontStart = FNTW(2);
	fontHeight = FNTB(6);
}

uint32_t Display::DrawChar(uint32_t xx, uint32_t yy, char c, uint16_t color)
{
	if (c == 32)
	{
		return fontHeight >> 2;  // Space is 1/4 font height (yuk);
	}

	uint32_t offset;
	uint8_t width;
	offset = (c - fontStart);
	offset = offset * 4;
	offset += 8;
	width = FNTB(offset);
	offset = FNTW(offset + 1);

	for (int y = 0; y < fontHeight; y++)
	{
		uint8_t p = 0;
		for (uint8_t x = 0; x < (uint8_t)width; x++)
		{
			if (x % 8 == 0)
			{
				p = FNTB(offset++);
			}
			if(p & 0x01)
			{
				//ILI9341.Rectangle(x + xx, y + yy, 1, 1, color);
				Pixel(x + xx, y + yy, color);
			}
			p >>= 1;
		}
	}
	return width;
}

uint32_t Display::DrawString(const char* s, uint32_t len, uint32_t x, uint32_t y, uint16_t color)
{
	for (uint16_t i = 0; i < len; i++)
	{
		x += DrawChar(x, y, s[i], color) + 1;
	}
	return x;
}

uint32_t Display::DrawString(const char* s, uint32_t x, uint32_t y, uint16_t color)
{
	return DrawString(s, strlen(s), x, y, color);
}

void Display::Label(const char* s, TextAlign align, uint32_t x, uint32_t y, uint32_t width, uint16_t fgColor, uint16_t bgColor)
{
	uint16_t hLineBuf[512];
	uint32_t length = strlen(s);
	uint32_t strWidth = MeasureString(s, length);
	uint32_t start = 0;
	uint32_t space = width - strWidth;

	switch (align)
	{
	case TextAlignLeft:
		start = x;
		Rect(x + strWidth, y, space, fontHeight, bgColor);
		break;
	case TextAlignRight:
		start = x + space;
		Rect(x, y, space, fontHeight, bgColor);
		break;
	case TextAlignCenter:
		start = x + space / 2;
		Rect(x, y, space / 2, fontHeight, bgColor);
		Rect(start + strWidth, y, space / 2, fontHeight, bgColor);
		break;
	}

	while (length--)
	{
		char c = *s++;
		if (c == 32)
		{
			Rect(start, y, fontHeight >> 2, fontHeight, bgColor);
			start += fontHeight >> 2;  // Space is 1/4 font height (yuk);
			continue;
		}

		uint32_t cOffset = (c - fontStart) * 4 + 8;
		uint8_t cWidth = FNTB(cOffset);
		cOffset = FNTW(cOffset + 1);
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

		SetBounds(start, y, cWidth, fontHeight);
		Blit16(hLineBuf, fontHeight * cWidth, false);
		Rect(start + cWidth, y, 1, fontHeight, bgColor);

		start += cWidth + 1;
	}
}

uint8_t Display::measureChar(char c)
{
	if (c == 32)
	{
		return fontHeight >> 2;  // Space is 1/4 font height (yuk);
	}

	uint32_t offset;
	offset = (c - fontStart);
	offset = offset * 4;
	offset += 8;
	return FNTB(offset);
}

uint32_t Display::MeasureString(const char* s, uint32_t len)
{
	uint32_t x = 0;
	while (len--)
	{
		x += measureChar(*s++) + 1;
	}
	return x;
}

uint32_t Display::MeasureString(const char* s)
{
	return MeasureString(s, strlen(s));
}

void Display::Draw4(uint32_t cx, uint32_t cy, uint32_t x, uint32_t y, uint16_t color)
{
	Pixel(cx + x, cy + y, color);
	if (x != 0) Pixel(cx - x, cy + y, color);
	if (y != 0) Pixel(cx + x, cy - y, color);
	Pixel(cx - x, cy - y, color);
}

void Display::Draw8(uint32_t cx, uint32_t cy, uint32_t x, uint32_t y, uint16_t color, bool fill)
{
	if (fill)
	{
		Rect(cx - y, cy - x, y << 1, 1, color);
		Rect(cx - x, cy - y, x << 1, 1, color);
		Rect(cx - x, cy + y, x << 1, 1, color);
		Rect(cx - y, cy + x, y << 1, 1, color);
	}
	else
	{
		Draw4(cx, cy, x, y, color);
		if (x != y)
		{
			Draw4(cx, cy, y, x, color);
		}
	}
}

void Display::Circle(uint32_t cx, uint32_t cy, uint32_t radius, uint16_t color, bool fill)
{
	int error = -radius;
	uint32_t x = radius;
	uint32_t y = 0;

//	draws in 4 strips from top down and middle up + reflection about y
	while (x >= y)
	{
		Draw8(cx, cy, x, y, color, fill);
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

void Display::Line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color)
{
    if(x0 == x1) // Vertical Line
    {
        if(y0 > y1) _swap_uint32_t(y0, y1);
        Rect(x0, y0, 1, y1 - y0 + 1, color);
        return;
    }
    else if(y0 == y1) // Horizontal Line
    {
        if(x0 > x1) _swap_uint32_t(x0, x1);
        Rect(x0, y0, x1 - x0 + 1, 1, color);
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
            Pixel(y0, x0, color);
        }
        else
        {
        	Pixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

void Display::Triangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint16_t color, bool fill)
{
	if (!fill)
	{
		Line(x0, y0, x1, y1, color);
	    Line(x1, y1, x2, y2, color);
	    Line(x2, y2, x0, y0, color);
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
		Rect(a, y0, b-a+1, 1, color);
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
		Rect(a, y, b-a+1, 1, color);
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
		Rect(a, y, b-a+1, 1, color);
	}
}

void Display::SetOrientationSize(DisplayOrientation orientation, uint32_t width, uint32_t height)
{
	displayOrientation = orientation;
	displayWidth = width;
	displayHeight = height;
}
