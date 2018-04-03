/*
 * @file spi.c
 * @brief implementation of spi.h
 *
 * spi read, write, send, and flush
 *
 *  @author Seth Miers and Jake Cazden
 *  @date March 15, 2018
 */

void SPI_init()
{
	/* TODO implement function */

	/* Enable the clock for the SPI */
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;

	/* configure output pins */
	PORTD->PCR[0] = PORT_PCR_MUX(2); /*PCS*/
	PORTD->PCR[1] = PORT_PCR_MUX(2); /*SCLK*/
	PORTD->PCR[2] = PORT_PCR_MUX(2); /*MOSI*/
	PORTD->PCR[3] = PORT_PCR_MUX(2); /*MISO*/

	/* set to master mode, enable spi, enable cpha */
	SPI0->C1 = ((1<<SPI_C1_SPE_SHIFT)|(1<<SPI_C1_MSTR_SHIFT)|(1<<SPI_C1_CPHA_SHIFT));
	/* set data mode */
	SPI0->C2 = (1<<SPI_C2_SPISWAI_SHIFT);
	/* set baud rate */
	SPI0->BR = SPI_BR_SPPR(0x00);
}

void SPI_read_byte(uint8_t * byte)
{
	SPI0->D = 0x00;
	/* wait until byte exists*/
	while(SPI_status() & 0x80 != 0x80);
	*byte = SPI0->D;
}

void SPI_write_byte(uint8_t byte)
{
	/* wait until empty*/
	while((SPI_status() & 0x20) != 0x20);
	SPI0->D = byte;
}

void SPI_send_packet(uint8_t * p, size_t length)
{
	int i;
	uint8_t byte;
	for(i = 0; i < length; i++)
		SPI_write_byte(p[i]);
}

void SPI_flush()
{
	/* TODO implement function */
}
