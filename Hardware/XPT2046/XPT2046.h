// Copyright 2019 by Sven Fabricius, see LICENSE.txt
/**
 * @file XPT2046.h
 *
 * @brief XPT2046 accessing functionalities
 *
 * @author Sven Fabricius
 * Contact: Sven.Fabricius@livediesel.de
 *
 */

#ifndef HARDWARE_XPT2046_XPT2046_H_
#define HARDWARE_XPT2046_XPT2046_H_

// DriverLib Includes
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define XPT2046_DATA_TRANSFER_LENGTH		9

/**
 * interfacing class for accessing XPT2046 Touch controller
 */
class XPT2046 {
private:
	void * spi;
	uint16_t ifgOffset;

	uint8_t rxData[XPT2046_DATA_TRANSFER_LENGTH];

	volatile uint16_t * ctrlOut;
	volatile uint16_t * txbuf;
	volatile uint16_t * rxbuf;
	uint16_t csPin;

	uint32_t txDma;
	uint32_t rxDma;
	uint32_t dmaIrq;

	void setSpiPort(DIO_PORT_Interruptable_Type * ctrlPort, uint16_t csPin, uint16_t sckPin, uint16_t siPin, uint16_t soPin);

public:
	XPT2046(EUSCI_B_SPI_Type * spi, DIO_PORT_Interruptable_Type * ctrlPort, uint16_t csPin, uint16_t sckPin, uint16_t siPin, uint16_t soPin);
	XPT2046(EUSCI_A_SPI_Type * spi, DIO_PORT_Interruptable_Type * ctrlPort, uint16_t csPin, uint16_t sckPin, uint16_t siPin, uint16_t soPin);

	void enableDMA(uint32_t txDma, uint32_t rxDma, uint32_t dmaIrq);
	void DMAIRQ(void);
	void transferDMA(void);
};

#endif /* HARDWARE_XPT2046_XPT2046_H_ */
