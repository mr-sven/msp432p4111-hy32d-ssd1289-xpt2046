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

#define SSD1289_OSC_START               0x0000
#define SSD1289_OSC_START_OSCEN         BIT(0)

#define SSD1289_DEVICE_CODE_READ        0x0000
#define SSD1289_DRV_OUT_CTRL            0x0001
#define SSD1289_LCD_DRV_AC_CTRL         0x0002
#define SSD1289_PWR_CTRL_1              0x0003
#define SSD1289_COMP_REG_1              0x0005
#define SSD1289_COMP_REG_2              0x0006
#define SSD1289_DISP_CTRL               0x0007
#define SSD1289_FRM_CYL_CTRL            0x000B
#define SSD1289_PWR_CTRL_2              0x000C
#define SSD1289_PWR_CTRL_3              0x000D
#define SSD1289_PWR_CTRL_4              0x000E
#define SSD1289_GAT_SCN_STRT_POS        0x000F
#define SSD1289_SLEEP_MODE              0x0010
#define SSD1289_ENTRY_MODE              0x0011
#define SSD1289_GEN_INTF_CTRL           0x0015
#define SSD1289_HORIZ_PORCH             0x0016
#define SSD1289_VERT_PORCH              0x0017
#define SSD1289_PWR_CTRL_5              0x001E
#define SSD1289_RAM_RW                  0x0022
#define SSD1289_RAM_WR_MASK_1           0x0023
#define SSD1289_RAM_WR_MASK_2           0x0024
#define SSD1289_FRM_FREQ                0x0025
#define SSD1289_VCOM_OTP_1              0x0028
#define SSD1289_VCOM_OTP_2              0x0029
#define SSD1289_GAMMA_CTRL_1            0x0030
#define SSD1289_GAMMA_CTRL_2            0x0031
#define SSD1289_GAMMA_CTRL_3            0x0032
#define SSD1289_GAMMA_CTRL_4            0x0033
#define SSD1289_GAMMA_CTRL_5            0x0034
#define SSD1289_GAMMA_CTRL_6            0x0035
#define SSD1289_GAMMA_CTRL_7            0x0036
#define SSD1289_GAMMA_CTRL_8            0x0037
#define SSD1289_GAMMA_CTRL_9            0x003A
#define SSD1289_GAMMA_CTRL_10           0x003B
#define SSD1289_VERT_SCROLL_1           0x0041
#define SSD1289_VERT_SCROLL_2           0x0042
#define SSD1289_HORIZ_RAM_ADDR_POS      0x0044
#define SSD1289_VERT_RAM_ADDR_STRT_POS  0x0045
#define SSD1289_VERT_RAM_ADDR_END_POS   0x0046
#define SSD1289_FIRST_WND_START         0x0048
#define SSD1289_FIRST_WND_END           0x0049
#define SSD1289_SND_WND_START           0x004A
#define SSD1289_SND_WND_END             0x004B
#define SSD1289_SET_GDDRAM_X_ADDR_CNT   0x004E
#define SSD1289_SET_GDDRAM_Y_ADDR_CNT   0x004F

#endif /* HARDWARE_SSD1289_SSD1289_CMD_H_ */
