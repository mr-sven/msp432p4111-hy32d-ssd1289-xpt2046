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

/// number of samples to read
#define XPT2046_VALID_SAMPLES				5

/**
 * struct for accessing sample values
 */
typedef struct _XPT2046_Sample
{
	uint16_t x;
	uint16_t y;
	uint16_t z;
} XPT2046_Sample;

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

	/// index of the current sample to read
	uint8_t currentSample;

	/// samples to read left
	uint8_t sampleReadCount;

	/// sample list
	XPT2046_Sample samples[XPT2046_VALID_SAMPLES];

	/// current reading state
	bool readingSamples;

	/// is current sample valid
	bool sampleValid;

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

	/**
	 * Start DMA transfer
	 */
	void transferDMA(void);

	/**
	 * Calculate current sample from buffer
	 *
	 * @param sample result if sample is valid
	 * @return true if sample is valid
	 */
	bool calculateSample(XPT2046_Sample * sample);
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
	 * Starts a reading cycle
	 */
	void readSamples(void);

	/**
	 * Returns a valid touch sample
	 *
	 * @param sample the valid sample
	 * @return true if sample is valid
	 */
	bool getTouchSample(XPT2046_Sample * sample);
};

#endif /* HARDWARE_XPT2046_XPT2046_H_ */
