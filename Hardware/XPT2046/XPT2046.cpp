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

#define GET_RX_DATA(n)			(((rxData[n]<<8) | rxData[n+1]) >> XPT2046_VALUE_SHIFT & XPT2046_VALUE_MASK)

/// command sequence
static const uint8_t xpt2046_command_seq [] = {
	XPT2046_CMD | XPT2046_CMD_ADDR(XPT2046_ADDR_X),
	0x00,
	XPT2046_CMD | XPT2046_CMD_ADDR(XPT2046_ADDR_Z1),
	0x00,
	XPT2046_CMD | XPT2046_CMD_ADDR(XPT2046_ADDR_Z2),
	0x00,
	XPT2046_CMD | XPT2046_CMD_ADDR(XPT2046_ADDR_Y),
	0x00,
	0x00
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
	setSpiPort(ctrlPort, csPin, sckPin, siPin, soPin);

	spi->CTLW0 |= EUSCI_B_CTLW0_SWRST;
	spi->CTLW0 |= EUSCI_B_CTLW0_SYNC | EUSCI_B_CTLW0_MST | EUSCI_B_CTLW0_MSB | EUSCI_B_CTLW0_SSEL__SMCLK  | EUSCI_B_CTLW0_CKPH;
	//spi->IE |= EUSCI_B_IE_TXIE;
	spi->BRW = (48 / 2); // 2MHz
	spi->CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);

	txbuf = &spi->TXBUF;
	rxbuf = &spi->RXBUF;
}

XPT2046::XPT2046(EUSCI_A_SPI_Type * spi, DIO_PORT_Interruptable_Type * ctrlPort, uint16_t csPin, uint16_t sckPin, uint16_t siPin, uint16_t soPin)
{
	setSpiPort(ctrlPort, csPin, sckPin, siPin, soPin);

	// init SPI register
	spi->CTLW0 |= EUSCI_A_CTLW0_SWRST;
	spi->CTLW0 |= EUSCI_A_CTLW0_SYNC | EUSCI_A_CTLW0_MST | EUSCI_A_CTLW0_MSB | EUSCI_A_CTLW0_SSEL__SMCLK  | EUSCI_A_CTLW0_CKPH;
	//spi->IE |= EUSCI_A_IE_TXIE;
	spi->BRW = (48 / 2); // 2MHz
	spi->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);

	txbuf = &spi->TXBUF;
	rxbuf = &spi->RXBUF;
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

void XPT2046::DMAIRQ(void)
{
    MAP_DMA_clearInterruptFlag(rxDma & DMA_CHANNEL_MASK);

	// unselect touch controller
	*ctrlOut |= csPin;

	uint16_t x = GET_RX_DATA(XPT2046_X_OFF);
	uint16_t z1 = GET_RX_DATA(XPT2046_Z1_OFF);
	uint16_t z2 = GET_RX_DATA(XPT2046_Z2_OFF);
	uint16_t y = GET_RX_DATA(XPT2046_Y_OFF);

	if (z1 == 0)
	{
		z1 = 1;
	}
	double pessure = ( (double)x / 4096.0) * (( (double)z2 / (double)z1) - 1.0);
	if (pessure == 0)
	{
		return;
	}

	uint16_t result = pessure * 100.0;

	result = 0xff - (result & 0xff);

	if (result < 8)
	{
		return;
	}
}

void XPT2046::transferDMA(void)
{
	*ctrlOut &= ~(csPin);

    MAP_DMA_setChannelTransfer(txDma, UDMA_MODE_BASIC, (void *)xpt2046_command_seq, (void *)txbuf, XPT2046_DATA_TRANSFER_LENGTH);
    MAP_DMA_setChannelTransfer(rxDma, UDMA_MODE_BASIC, (void *)rxbuf, rxData, XPT2046_DATA_TRANSFER_LENGTH);

    MAP_DMA_enableChannel(rxDma & DMA_CHANNEL_MASK);
    MAP_DMA_enableChannel(txDma & DMA_CHANNEL_MASK);
}

