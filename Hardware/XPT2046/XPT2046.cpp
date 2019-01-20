// Copyright 2019 by Sven Fabricius, see LICENSE.txt
/**
 * @file XPT2046.cpp
 *
 * @brief XPT2046 accessing functionalities
 *
 * @author Sven Fabricius
 * Contact: Sven.Fabricius@livediesel.de
 *
 */

#include <Hardware/XPT2046/XPT2046.h>

#include <Hardware/XPT2046/XPT2046_cmd.h>

/// Channel Mask for extracting DMA channel
#define DMA_CHANNEL_MASK		(DMA_CHANNEL_0|DMA_CHANNEL_1|DMA_CHANNEL_2|DMA_CHANNEL_3|DMA_CHANNEL_4|DMA_CHANNEL_5|DMA_CHANNEL_6|DMA_CHANNEL_7)

/// rx data offset for X value data
#define XPT2046_X_OFF			1

/// rx data offset for Z1 value data
#define XPT2046_Z1_OFF			3

/// rx data offset for Z2 value data
#define XPT2046_Z2_OFF			5

/// rx data offset for Y value data
#define XPT2046_Y_OFF			7

/// value mask for data
#define XPT2046_VALUE_MASK		0xfff

/// value shift for data
#define XPT2046_VALUE_SHIFT		3

/// get data by index from rx buffer
#define GET_RX_DATA(n)			(((rxData[n]<<8) | rxData[n+1]) >> XPT2046_VALUE_SHIFT & XPT2046_VALUE_MASK)

/// maximum SPI clock speed
#define XPT2046_MAX_SPI_SPEED	2000000

/// selected SPI speed
#define XPT2046_SPI_SPEED		(CS_48MHZ / XPT2046_MAX_SPI_SPEED)

/// command sequence
static const uint8_t xpt2046_command_seq [] = {
	XPT2046_CMD | XPT2046_CMD_ADDR(XPT2046_ADDR_X),		// transmit read X command
	0x00,												// read higher X bits
	XPT2046_CMD | XPT2046_CMD_ADDR(XPT2046_ADDR_Z1),	// transmit read Z1 command and read lower X bits
	0x00,												// read higher Z1 bits
	XPT2046_CMD | XPT2046_CMD_ADDR(XPT2046_ADDR_Z2),	// transmit read Z2 command and read lower Z1 bits
	0x00,												// read higher Z2 bits
	XPT2046_CMD | XPT2046_CMD_ADDR(XPT2046_ADDR_Y),		// transmit read Y command and read lower Z2 bits
	0x00,												// read higher Y bits
	0x00												// read lower Y bits
};

void XPT2046::setSpiPort(DIO_PORT_Interruptable_Type * ctrlPort, uint16_t csPin, uint16_t sckPin, uint16_t siPin, uint16_t soPin)
{
	ctrlOut = &ctrlPort->OUT;
	this->csPin = csPin;

	// enable SPI on port Selector
	//                  CLK      MOSI    MISO
	ctrlPort->SEL0 |=   sckPin | siPin | soPin;
	ctrlPort->SEL1 &= ~(sckPin | siPin | soPin);

	// set CLK and MOSI to output
	ctrlPort->DIR |=   csPin | sckPin | siPin;

	// set MISO to input
	ctrlPort->DIR &= ~(soPin);

	// unselect touch controller
	*ctrlOut |= csPin;
}

XPT2046::XPT2046(EUSCI_B_SPI_Type * spi, DIO_PORT_Interruptable_Type * ctrlPort, uint16_t csPin, uint16_t sckPin, uint16_t siPin, uint16_t soPin)
{
	// setup io pins
	setSpiPort(ctrlPort, csPin, sckPin, siPin, soPin);

	// init SPI register
	spi->CTLW0 |= EUSCI_B_CTLW0_SWRST;
	spi->CTLW0 |= EUSCI_B_CTLW0_SYNC | EUSCI_B_CTLW0_MST | EUSCI_B_CTLW0_MSB | EUSCI_B_CTLW0_SSEL__SMCLK | EUSCI_B_CTLW0_CKPH;
	spi->BRW = XPT2046_SPI_SPEED;
	spi->CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);

	// store SPI buffers
	txbuf = &spi->TXBUF;
	rxbuf = &spi->RXBUF;

	// init var
	readingSamples = false;
}

XPT2046::XPT2046(EUSCI_A_SPI_Type * spi, DIO_PORT_Interruptable_Type * ctrlPort, uint16_t csPin, uint16_t sckPin, uint16_t siPin, uint16_t soPin)
{
	// setup io pins
	setSpiPort(ctrlPort, csPin, sckPin, siPin, soPin);

	// init SPI register
	spi->CTLW0 |= EUSCI_A_CTLW0_SWRST;
	spi->CTLW0 |= EUSCI_A_CTLW0_SYNC | EUSCI_A_CTLW0_MST | EUSCI_A_CTLW0_MSB | EUSCI_A_CTLW0_SSEL__SMCLK | EUSCI_A_CTLW0_CKPH;
	spi->BRW = XPT2046_SPI_SPEED;
	spi->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);

	// store SPI buffers
	txbuf = &spi->TXBUF;
	rxbuf = &spi->RXBUF;

	// init var
	readingSamples = false;
}

void XPT2046::enableDMA(uint32_t txDma, uint32_t rxDma, uint32_t dmaIrq)
{
	this->txDma = txDma; // DMA_CH0_EUSCIB1TX3
	this->rxDma = rxDma; // DMA_CH1_EUSCIB1RX3
	this->dmaIrq = dmaIrq; //DMA_INT1

    /* Assign DMA channel 0 to EUSCI_B0_TX0, channel 1 to EUSCI_B0_RX0 */
    MAP_DMA_assignChannel(txDma);
    MAP_DMA_assignChannel(rxDma);

    // Setup the TX transfer characteristics & buffers
    MAP_DMA_setChannelControl(txDma, UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_1);

    // Setup the RX transfer characteristics & buffers
    MAP_DMA_setChannelControl(rxDma, UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_1);

    // Enable DMA interrupt
    MAP_DMA_assignInterrupt(dmaIrq, rxDma & DMA_CHANNEL_MASK);
    MAP_DMA_clearInterruptFlag(rxDma & DMA_CHANNEL_MASK);
}

bool XPT2046::calculateSample(XPT2046_Sample * sample)
{
	// extract data from result buffer
	uint16_t x = GET_RX_DATA(XPT2046_X_OFF);
	uint16_t z1 = GET_RX_DATA(XPT2046_Z1_OFF);
	uint16_t z2 = GET_RX_DATA(XPT2046_Z2_OFF);
	uint16_t y = GET_RX_DATA(XPT2046_Y_OFF);

	// prevent zero division
	if (z1 == 0)
	{
		z1 = 1;
	}

	// calculate pressure
	double pessure = ( (double)x / 4096.0) * (( (double)z2 / (double)z1) - 1.0);

	// check if is touched
	if (pessure == 0)
	{
		return false;
	}

	// calculate pressure value
	uint16_t result = pessure * 100.0;
	result = 0xff - (result & 0xff);

	// check if pressure is to low
	if (result < 8)
	{
		return false;
	}

	// save samples
	sample->x = x;
	sample->y = y;
	sample->z = result;

	return true;
}

void XPT2046::DMAIRQ(void)
{
    MAP_DMA_clearInterruptFlag(rxDma & DMA_CHANNEL_MASK);

	// unselect touch controller
	*ctrlOut |= csPin;

	// calculate samples from buffer
	bool res = calculateSample(&samples[currentSample]);

	// check if sample is valid an minimum samples read
	if (res && ++currentSample == XPT2046_VALID_SAMPLES)
	{
		readingSamples = false;
		sampleValid = true;
		return;
	}

	// check if maximum sample read is reached
	if (--sampleReadCount == 0)
	{
		readingSamples = false;
		return;
	}

	// read next sample
	transferDMA();
}

void XPT2046::transferDMA(void)
{
	// enable CS pin
	*ctrlOut &= ~(csPin);

	// set transfer channel address
    MAP_DMA_setChannelTransfer(txDma, UDMA_MODE_BASIC, (void *)xpt2046_command_seq, (void *)txbuf, XPT2046_DATA_TRANSFER_LENGTH);
    MAP_DMA_setChannelTransfer(rxDma, UDMA_MODE_BASIC, (void *)rxbuf, rxData, XPT2046_DATA_TRANSFER_LENGTH);

    // enable DMA channels
    MAP_DMA_enableChannel(rxDma & DMA_CHANNEL_MASK);
    MAP_DMA_enableChannel(txDma & DMA_CHANNEL_MASK);
}

void XPT2046::readSamples(void)
{
	// return if samples are read
	if (readingSamples)
	{
		return;
	}

	// set current reading state
	readingSamples = true;

	// invalidate sample
	sampleValid = false;

	// set current sample pointer
	currentSample = 0;

	// set maximum samples to read
	sampleReadCount = XPT2046_VALID_SAMPLES * 2;

	// start DMA transfer
	transferDMA();
}

bool XPT2046::getTouchSample(XPT2046_Sample * sample)
{
	if (!sampleValid)
	{
		return false;
	}

	// Get the median of 5 valid samples
	for (uint8_t i = 0; i < 3; i++)
	{
		for (uint8_t j = i+1; j < XPT2046_VALID_SAMPLES; j++)
		{
			if (samples[i].x > samples[j].x)
			{
				uint16_t t = samples[i].x;
				samples[i].x = samples[j].x;
				samples[j].x = t;
			}

			if (samples[i].y > samples[j].y)
			{
				uint16_t t = samples[i].y;
				samples[i].y = samples[j].y;
				samples[j].y = t;
			}

			if (samples[i].z > samples[j].z)
			{
				uint16_t t = samples[i].z;
				samples[i].z = samples[j].z;
				samples[j].z = t;
			}
		}
	}

	sample->x = samples[2].x;
	sample->y = samples[2].y;
	sample->z = samples[2].z;

	return true;
}
