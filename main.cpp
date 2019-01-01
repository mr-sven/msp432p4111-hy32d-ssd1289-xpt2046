
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Core Includes */
#include <Core/System.h>
#include <Core/IO.h>

#include <Hardware/SSD1289/SSD1289.h>

#include <Colors.h>

#define ORIENT_270

/* Timer_A PWM Configuration Parameter */
const Timer_A_PWMConfig pwmConfig =
{
	.clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
	.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1,
    .timerPeriod = PWM_PERIOD,
	.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1,
    .compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET,
    .dutyCycle = (uint_fast16_t)(PWM_PERIOD * LED_PWM)
};

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

int main(void)
{
    // Stop Watchdog
    MAP_WDT_A_holdTimer();

    // Setup Clock Sources
    System.SetupCS();

    // Setup System Tick Timer
    System.SetupSystickTimer();

    // Reset Output
    IO.SetupOutputs();

    // Disable global interrupt
    MAP_Interrupt_disableMaster();

    // Enable global interrupt
    MAP_Interrupt_enableMaster();

    setupDisplayLEDPWM();


    SSD1289 ssd(LCD_DATA, LCD_CTRL, LCD_CS, LCD_RS, LCD_RD, LCD_WR);

    LCD_CTRL->DIR |= LCD_RESET;
    LCD_CTRL->OUT &= ~(LCD_RESET);
    System.DelayMs(20);
    LCD_CTRL->OUT |= LCD_RESET;
    System.DelayMs(20);

#ifdef ORIENT_0
    ssd.Init(DisplayOrientation_0, 240, 320);
#elif defined ORIENT_90
    ssd.Init(DisplayOrientation_90, 320, 240);
#elif defined ORIENT_180
    ssd.Init(DisplayOrientation_180, 240, 320);
#elif defined ORIENT_270
    ssd.Init(DisplayOrientation_270, 320, 240);
#endif

#if defined(ORIENT_0) || defined(ORIENT_180)
    ssd.SetBounds(0, 0, 240, 320);
    ssd.Fill(clRed, 240*320);
#elif defined(ORIENT_90) || defined(ORIENT_270)
    ssd.SetBounds(0, 0, 320, 240);
#endif

    ssd.Fill(clRed, 240*320);

    ssd.SetBounds(1, 2, 10, 20);
    ssd.Fill(clWhite, 10*20);
    ssd.SetBounds(1, 2, 10, 20);
    ssd.Fill(clBlack, 10*20-55);

    while(1)
    {

        MAP_PCM_gotoLPM0();
    }
}
