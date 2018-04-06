/*
 * @file spi.c
 * @brief implementation of spi.h
 *
 * spi read, write, send, and flush
 *
 *  @author Seth Miers and Jake Cazden
 *  @date March 15, 2018
 */

#include "spi.h"
#include "MKL25Z4.h"

/* TODO move chip enable to function in spi.c */

void SPI_init()
{
	/* TODO implement function */

	/* Enable the clock for the SPI */
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;

	/* configure output pins */
	PORTD_PCR0 = PORT_PCR_MUX(1); /*CE*/
    GPIOD_PDDR |= (1 << 0); /* set to output */
    GPIOD_PSOR |= (1 << 0); /* set high (off) */
	PORTD_PCR1 = PORT_PCR_MUX(2); /*SCLK*/
	PORTD_PCR2 = PORT_PCR_MUX(2); /*MOSI*/
	PORTD_PCR3 = PORT_PCR_MUX(2); /*MISO*/
	/*PORTD_PCR5 = PORT_PCR_MUX(1); CSN*/
    /*GPIOD_PDDR |= (1 << 5); set to output */

	/* set as master device: 0x10 */
	SPI0_C1 = 0x10;
	/* auto control CE */
	SPI0_C2 = 0x00;
	/* set the baud rate divisor to 64 */
	SPI0_BR = 0x0A;
	/* enable spi */
	SPI0_C1 |= 0x40;
}

void SPI_read_byte(uint8_t * byte)
{
	/* wait until empty*/
	while((SPI0_S & SPI_S_SPTEF_MASK) != SPI_S_SPTEF_MASK);
	SPI0_D = 0x00;
	/* wait until byte exists*/
	while((SPI0_S & SPI_S_SPRF_MASK) != SPI_S_SPRF_MASK);
	*byte = SPI0_D;
}

void SPI_write_byte(uint8_t byte)
{
	/* wait until empty*/
	while((SPI0_S & SPI_S_SPTEF_MASK) != SPI_S_SPTEF_MASK);
	SPI0_D = byte;
	/* wait until byte exists in order to block*/
	while((SPI0_S & SPI_S_SPRF_MASK) != SPI_S_SPRF_MASK);
	/* discard byte */
	byte = SPI0_D;
}

void SPI_send_packet(uint8_t * p, size_t length)
{
	uint8_t i;
	for(i = 0; i < length; i++)
		SPI_write_byte(p[i]);
}

void SPI_flush()
{
	/* TODO implement function */
}
