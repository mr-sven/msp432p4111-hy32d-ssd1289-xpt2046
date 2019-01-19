# Interfacing HY32D Display with MSP432

This repository contains a set of libs to interface the HY32D display using 16Bit parallel interface to the SSD1289 display controller and SPI interface to the XPT2046 touch controller.

Tasks:
- [x] base code structure
- [x] abstract display class
- [x] SSD1289 interface class
- [x] SSD1289 assembler functions for better performance
- [ ] XPT2046 touch controller interface
  - [x] SPI read via DMA
  - [x] measure multiple times
  - [ ] touch down detection
  - [ ] calibration
- [x] code documentation

## Hardware specification

* Display HY32D
  * Controller: SSD1289
  * Interface: 16Bit parallel + 5 control pins
  * Touch Controller: XPT2046
  * Interface: 4-Wire SPI + 1 IRQ
  * Backlight PWM
* µC MSP432p4111

## Wiring

The connected pins and interfaces can be modified in the [Board.h](Board.h).

Current Connections:
* LCD Data: Port B (P3 + P4)
* LCD Control: Port C (P5 + P6)
  * LCD Reset: PC.0 (P5.0)
  * LCD RS: PC.1 (P5.1)
  * LCD WR: PC.2 (P5.2)
  * LCD RD: PC.3 (P5.3)
  * LCD CS: PC.4 (P5.4)
* Backlight PWM: P2.0
* Touch SPI: EUSCI_B1_SPI
  * Touch IRQ: P6.1
  * Touch CS: P6.2
  * Touch SCK: P6.3
  * Touch SI: P6.4
  * Touch SO: P6.5

## Code

The source is based on Code Composer Studio R8.0 including the SimpleLink SDK 2.30.00.14.

