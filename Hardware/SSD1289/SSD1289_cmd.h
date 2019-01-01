/*
 * SSD1289_cmd.h
 *
 *  Created on: 26.12.2018
 *      Author: Sven
 */

#ifndef HARDWARE_SSD1289_SSD1289_CMD_H_
#define HARDWARE_SSD1289_SSD1289_CMD_H_

#define _DELAY_MS               0xFF00
#define _END                    0xFFFF

#define OSC_START               0x0000
#define DEVICE_CODE_READ        0x0000
#define DRV_OUT_CTRL            0x0001
#define LCD_DRV_AC_CTRL         0x0002
#define PWR_CTRL_1              0x0003
#define COMP_REG_1              0x0005
#define COMP_REG_2              0x0006
#define DISP_CTRL               0x0007
#define FRM_CYL_CTRL            0x000B
#define PWR_CTRL_2              0x000C
#define PWR_CTRL_3              0x000D
#define PWR_CTRL_4              0x000E
#define GAT_SCN_STRT_POS        0x000F
#define SLEEP_MODE              0x0010
#define ENTRY_MODE              0x0011
#define GEN_INTF_CTRL           0x0015
#define HORIZ_PORCH             0x0016
#define VERT_PORCH              0x0017
#define PWR_CTRL_5              0x001E
#define RAM_RW                  0x0022
#define RAM_WR_MASK_1           0x0023
#define RAM_WR_MASK_2           0x0024
#define FRM_FREQ                0x0025
#define VCOM_OTP_1              0x0028
#define VCOM_OTP_2              0x0029
#define GAMMA_CTRL_1            0x0030
#define GAMMA_CTRL_2            0x0031
#define GAMMA_CTRL_3            0x0032
#define GAMMA_CTRL_4            0x0033
#define GAMMA_CTRL_5            0x0034
#define GAMMA_CTRL_6            0x0035
#define GAMMA_CTRL_7            0x0036
#define GAMMA_CTRL_8            0x0037
#define GAMMA_CTRL_9            0x003A
#define GAMMA_CTRL_10           0x003B
#define VERT_SCROLL_1           0x0041
#define VERT_SCROLL_2           0x0042
#define HORIZ_RAM_ADDR_POS      0x0044
#define VERT_RAM_ADDR_STRT_POS  0x0045
#define VERT_RAM_ADDR_END_POS   0x0046
#define FIRST_WND_START         0x0048
#define FIRST_WND_END           0x0049
#define SND_WND_START           0x004A
#define SND_WND_END             0x004B
#define SET_GDDRAM_X_ADDR_CNT   0x004E
#define SET_GDDRAM_Y_ADDR_CNT   0x004F

#endif /* HARDWARE_SSD1289_SSD1289_CMD_H_ */
