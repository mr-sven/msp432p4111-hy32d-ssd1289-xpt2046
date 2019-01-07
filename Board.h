// Copyright 2019 by Sven Fabricius, see LICENSE.txt
/**
 * @file Board.h
 *
 * @brief Board Config
 *
 * @author Sven Fabricius
 * Contact: Sven.Fabricius@livediesel.de
 *
 */

#ifndef BOARD_H_
#define BOARD_H_

// GLUE
#define PASTER2(x,y)		x ## y
#define EVALUATOR2(x,y)		PASTER2(x,y)
#define PASTER3(x,y,z)		x ## y ## z
#define EVALUATOR3(x,y,z)	PASTER3(x,y,z)

#define PWM_PERIOD			(CS_48MHZ/5000)
#define LED_PWM				1.0 // 0.3

#define JTAG_ENABLED

#define LCD_DATA			PB
#define LCD_CTRL			PC
#define LCD_RESET			GPIO_PIN0
#define LCD_RS				GPIO_PIN1
#define LCD_WR				GPIO_PIN2
#define LCD_RD				GPIO_PIN3
#define LCD_CS				GPIO_PIN4

#define TOUCH_SPI			EUSCI_B1_SPI
#define TOUCH_PORT			PC
#define TOUCH_IRQ			GPIO_PIN9
#define TOUCH_CS			GPIO_PIN10
#define TOUCH_SCK			GPIO_PIN11
#define TOUCH_SI			GPIO_PIN12
#define TOUCH_SO			GPIO_PIN13

#endif /* BOARD_H_ */
