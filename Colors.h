/*
 * Colors.h
 *
 * Created: 20.09.2013 19:47:52
 *  Author: gg
 */ 


#ifndef COLORS_H_
#define COLORS_H_

#define TOCOLOR(_r,_g,_b) ((((_r) & 0xF8) << 8) | (((_g) & 0xFC) << 3) | (((_b) & 0xF8) >> 3))
#define GRAY(_g)		(TOCOLOR(_g,_g,_g))

enum Color : uint16_t {
	Black		= GRAY(0),
	DarkGray	= GRAY(96),
	Gray		= GRAY(128),
	LightGray	= GRAY(192),
	White		= GRAY(255),

	Red			= TOCOLOR(255,0,0),
	DarkGreen	= TOCOLOR(0,128,0),
	Green		= TOCOLOR(0,255,0),
	LightGreen	= TOCOLOR(144,255,144),
	DarkBlue	= TOCOLOR(0,0,128),
	Blue		= TOCOLOR(0,0,255),
	LightBlue	= TOCOLOR(144,144,255),
	Yellow		= TOCOLOR(255,255,0),
	Cyan		= TOCOLOR(0,255,255),
	Magenta		= TOCOLOR(255,0,255),

	StdBlue		= TOCOLOR(0,120,215),
	MeadowGreen	= TOCOLOR(73,130,5),
	Storm		= TOCOLOR(76,74,72),
	GrasGreen	= TOCOLOR(0,204,106),
	YellowGold	= TOCOLOR(255,185,5),
	SeaGreen	= TOCOLOR(0,183,195),
};

#endif /* COLORS_H_ */
