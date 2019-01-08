/*
 * XPT2046_cmd.h
 *
 *  Created on: 07.01.2019
 *      Author: Sven
 */

#ifndef HARDWARE_XPT2046_XPT2046_CMD_H_
#define HARDWARE_XPT2046_XPT2046_CMD_H_

#define XPT2046_CMD						0x80
#define XPT2046_CMD_ADDR(n)				(((n) & 7) << 4)
#define XPT2046_ADDR_TEMP0				0
#define XPT2046_ADDR_Y					1
#define XPT2046_ADDR_VBAT				2
#define XPT2046_ADDR_Z1					3
#define XPT2046_ADDR_Z2					4
#define XPT2046_ADDR_X					5
#define XPT2046_ADDR_AUXIN				6
#define XPT2046_ADDR_TEMP1				7
#define XPT2046_CMD_MODE_8BIT			BIT(3)
#define XPT2046_CMD_MODE_SINGLE_ENDED	BIT(2)
#define XPT2046_CMD_POWER_DOWN_MODE(n)	((n) & 3)

#endif /* HARDWARE_XPT2046_XPT2046_CMD_H_ */
