// Copyright 2019 by Sven Fabricius, see LICENSE.txt

// DriverLib Includes
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Standard Includes
#include <stdint.h>
#include <stdbool.h>

// Core Includes
#include <Core/System.h>
#include <Core/IO.h>

#include <Hardware/SSD1289/SSD1289.h>
#include <Hardware/XPT2046/XPT2046.h>

#include <Colors.h>
#include <Board.h>

#include <Fonts/Liberation_Sans21x23.h>

#define ORIENT_270

// Timer_A PWM Configuration Parameter
const Timer_A_PWMConfig pwmConfig =
{
	.clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
	.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1,
    .timerPeriod = PWM_PERIOD,
	.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1,
    .compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET,
    .dutyCycle = (uint_fast16_t)(PWM_PERIOD * LED_PWM)
};

/* DMA Control Table */
#if defined(__TI_COMPILER_VERSION__)
#ifdef __cplusplus
#pragma DATA_ALIGN(1024)
#else
#pragma DATA_ALIGN(MSP432P4111_DMAControlTable, 1024)
#endif
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=1024
#elif defined(__GNUC__)
__attribute__ ((aligned (1024)))
#elif defined(__CC_ARM)
__align(1024)
#endif
static DMA_ControlTable MSP432P4111_DMAControlTable[DMA_CONTROL_MEMORY_ALIGNMENT];

// Port mapper configuration register
const uint8_t port_mapping[] =
{
	//Port P2:
	PMAP_TA0CCR1A, PMAP_NONE, PMAP_NONE, PMAP_NONE, PMAP_NONE, PMAP_NONE, PMAP_NONE, PMAP_NONE
};

void setupDisplayLEDPWM(void)
{
	MAP_PMAP_configurePorts(port_mapping, PMAP_P2MAP, 1, PMAP_DISABLE_RECONFIGURATION);
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);
	MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
}

XPT2046 *touchP;

void DMATouchIRQHandler(void)
{
	touchP->DMAIRQ();
}

int main(void)
{
    // Stop Watchdog
    MAP_WDT_A_holdTimer();

    // Setup Clock Sources
    System.setupCS();

    // Setup System Tick Timer
    System.setupSystickTimer();

    // Reset Output
    IO.setupOutputs();

    // Disable global interrupt
    MAP_Interrupt_disableMaster();

    // Enable global interrupt
    MAP_Interrupt_enableMaster();

    // init PWM for background light
    setupDisplayLEDPWM();


    // Configuring DMA module
    MAP_DMA_enableModule();
    MAP_DMA_setControlBase(MSP432P4111_DMAControlTable);

    XPT2046 touch(TOUCH_SPI, TOUCH_PORT, TOUCH_CS, TOUCH_SCK, TOUCH_SI, TOUCH_SO);
    touch.enableDMA(DMA_CH2_EUSCIB1TX0, DMA_CH3_EUSCIB1RX0, DMA_INT1);
    touchP = &touch;

    MAP_DMA_registerInterrupt(DMA_INT1, DMATouchIRQHandler);
    MAP_Interrupt_enableInterrupt(DMA_INT1);
    MAP_DMA_enableInterrupt(DMA_INT1);

    touch.transferDMA();

    SSD1289 ssd(LCD_DATA, LCD_CTRL, LCD_CS, LCD_RS, LCD_RD, LCD_WR);

    // display Reset
    LCD_CTRL->DIR |= LCD_RESET;
    LCD_CTRL->OUT &= ~(LCD_RESET);
    System.delayMs(20);
    LCD_CTRL->OUT |= LCD_RESET;
    System.delayMs(20);

#ifdef ORIENT_0
    ssd.init(DisplayOrientation::Portrait, 240, 320);
#elif defined ORIENT_90
    ssd.init(DisplayOrientation::Landscape_90, 320, 240);
#elif defined ORIENT_180
    ssd.init(DisplayOrientation::Portrait_180, 240, 320);
#elif defined ORIENT_270
    ssd.init(DisplayOrientation::Landscape_270, 320, 240);
#endif

#if defined(ORIENT_0) || defined(ORIENT_180)
    ssd.setBounds(0, 0, 240, 320);
    ssd.fill(clRed, 240*320);
#elif defined(ORIENT_90) || defined(ORIENT_270)
    ssd.setBounds(0, 0, 320, 240);
#endif

    ssd.fill(Color::StdBlue, 240*320);

    ssd.setBounds(1, 2, 10, 20);
    ssd.fill(Color::White, 10*20);
    ssd.setBounds(1, 2, 10, 20);
    ssd.fill(Color::Black, 10*20-55);

    ssd.setFont(Liberation_Sans21x23);
    ssd.drawString("hello", 30, 20, Color::Black);

    ssd.label("Hello", TextAlign::Center, 100, 100, 100, Color::Magenta, Color::YellowGold);
    while(1)
    {
        MAP_PCM_gotoLPM0();
    }
}
