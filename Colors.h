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

#define clBtBrdTo		TOCOLOR(154,154,154)
#define clBtBrdBo		TOCOLOR(103,103,103)

#define clBlack			GRAY(0)
#define clDarkGray		GRAY(96)
#define clGray			GRAY(128)
#define clLightGray		GRAY(192)
#define clWhite			GRAY(255)

#define clRed			TOCOLOR(255,0,0)
#define clDarkGreen		TOCOLOR(0,128,0)
#define clGreen			TOCOLOR(0,255,0)
#define clLightGreen	TOCOLOR(144,255,144)
#define clDarkBlue		TOCOLOR(0,0,128)
#define clBlue			TOCOLOR(0,0,255)
#define clLightBlue		TOCOLOR(144,144,255)
#define clYellow		TOCOLOR(255,255,0)
#define clCyan			TOCOLOR(0,255,255)
#define clMagenta		TOCOLOR(255,0,255)

#define clStdBlue		TOCOLOR(0,120,215)
#define clMeadowGreen	TOCOLOR(73,130,5)
#define clStorm			TOCOLOR(76,74,72)
#define clGrasGreen		TOCOLOR(0,204,106)
#define clYellowGold	TOCOLOR(255,185,5)
#define clSeaGreen		TOCOLOR(0,183,195)

#endif /* COLORS_H_ */
