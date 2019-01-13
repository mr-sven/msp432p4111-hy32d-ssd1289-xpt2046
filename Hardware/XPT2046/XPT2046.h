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

/// length of the DMA transfer message
#define XPT2046_DATA_TRANSFER_LENGTH		9

/**
 * interfacing class for accessing XPT2046 Touch controller
 */
class XPT2046 {
private:
	/// DMA transfer receive buffer
	uint8_t rxData[XPT2046_DATA_TRANSFER_LENGTH];

	/// pointer to the control port
	volatile uint16_t * ctrlOut;

	/// pointer to the tx buffer of the SPI interface for later DMA access
	volatile uint16_t * txbuf;

	/// pointer to the rx buffer of the SPI interface for later DMA access
	volatile uint16_t * rxbuf;

	/// bitmask for the CS Pin at Control Port
	uint16_t csPin;

	/// TX DMA Channel
	uint32_t txDma;

	/// RX DMA Channel
	uint32_t rxDma;

	/// DMA Channel IRQ
	uint32_t dmaIrq;

	/**
	 * Configure the SPI port IOs
	 *
	 * @param ctrlPort the Control Port
	 * @param csPin bitmask for the CS Pin at Control Port
	 * @param sckPin bitmask for the SCK Pin at Control Port
	 * @param siPin bitmask for the SI Pin at Control Port
	 * @param soPin bitmask for the SO Pin at Control Port
	 */
	void setSpiPort(DIO_PORT_Interruptable_Type * ctrlPort, uint16_t csPin, uint16_t sckPin, uint16_t siPin, uint16_t soPin);

public:
	/**
	 * Constructor
	 *
	 * @param spi SPI port
	 * @param ctrlPort the Control Port
	 * @param csPin bitmask for the CS Pin at Control Port
	 * @param sckPin bitmask for the SCK Pin at Control Port
	 * @param siPin bitmask for the SI Pin at Control Port
	 * @param soPin bitmask for the SO Pin at Control Port
	 */
	XPT2046(EUSCI_B_SPI_Type * spi, DIO_PORT_Interruptable_Type * ctrlPort, uint16_t csPin, uint16_t sckPin, uint16_t siPin, uint16_t soPin);

	/**
	 * @overload
	 */
	XPT2046(EUSCI_A_SPI_Type * spi, DIO_PORT_Interruptable_Type * ctrlPort, uint16_t csPin, uint16_t sckPin, uint16_t siPin, uint16_t soPin);

	/**
	 * Enable the DMA configuration
	 *
	 * @param txDma TX DMA Channel
	 * @param rxDma RX DMA Channel
	 * @param dmaIrq DMA Channel IRQ
	 */
	void enableDMA(uint32_t txDma, uint32_t rxDma, uint32_t dmaIrq);

	/**
	 * Execute IRQ
	 */
	void DMAIRQ(void);

	/**
	 * Start DMA transfer
	 */
	void transferDMA(void);
};

#endif /* HARDWARE_XPT2046_XPT2046_H_ */
