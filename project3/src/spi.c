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
#ifdef KL25Z
#include "MKL25Z4.h"
#endif
#ifdef BBB
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#endif

/* TODO move chip enable to function in spi.c */

void SPI_init()
{
#ifdef KL25Z
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
#endif
#ifdef BBB
	/* TODO implement function */
#endif
}

void SPI_read_byte(uint8_t * byte)
{
#ifdef KL25Z
	/* wait until empty*/
	while((SPI0_S & SPI_S_SPTEF_MASK) != SPI_S_SPTEF_MASK);
	SPI0_D = 0x00;
	/* wait until byte exists*/
	while((SPI0_S & SPI_S_SPRF_MASK) != SPI_S_SPRF_MASK);
	*byte = SPI0_D;
#endif
#ifdef BBB
	/* TODO implement function */
#endif
}

void SPI_write_byte(uint8_t byte)
{
#ifdef KL25Z
	/* wait until empty*/
	while((SPI0_S & SPI_S_SPTEF_MASK) != SPI_S_SPTEF_MASK);
	SPI0_D = byte;
	/* wait until byte exists in order to block*/
	while((SPI0_S & SPI_S_SPRF_MASK) != SPI_S_SPRF_MASK);
	/* discard byte */
	byte = SPI0_D;
#endif
#ifdef BBB
	/* TODO implement function */
#endif
}

void SPI_send_packet(uint8_t * p, size_t length)
{
#ifdef KL25Z
	uint8_t i;
	for(i = 0; i < length; i++)
		SPI_write_byte(p[i]);
#endif
#ifdef BBB
	/* TODO implement function */
#endif
}

void SPI_flush()
{
#ifdef KL25Z
	/* TODO implement function */
#endif
#ifdef BBB
	/* TODO implement function */
#endif
}
