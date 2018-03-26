/*
 * @file nordic.c
 * @brief
 *
 *
 *
 *  @author Seth Miers and Jake Cazden
 *  @date March 15, 2018
 */

uint8_t nrf_read_register(uint8_t readRegister)
{
	uint8_t command = 0x1F & readRegister;
	SPI_write_byte(command);
	uint8_t readByte = 0x00;
	SPI_read_byte(&readByte);
	return readByte;
}

void nrf_write_register(uint8_t writeRegister, uint8_t value)
{
	uint8_t command = 0x3F & readRegister;
	SPI_write_byte(command);
	uint8_t readByte = 0x00;
	SPI_write_byte(value);
}

__attribute((always_inline))
uint8_t nrf_read_status()
{
	uint8_t command = 0xFF;
	SPI_write_byte(command);
	uint8_t readByte = 0x00;
	SPI_read_byte(&readByte);
	return readByte;
}

void nrf_write_config(uint8_t config)
{
	nrf_write_register(0x00, config & 0x7F);
}

uint8_t nrf_read_rf_ch()
{
	return nrf_read_register(0x06);
}

void nrf_read_tx_addr(uint8_t * address)
{
	uint8_t command = 0x10;
	SPI_write_byte(command);
	SPI_read_byte(address++);
	SPI_read_byte(address++);
	SPI_read_byte(address++);
	SPI_read_byte(address);
}

void nrf_write_tx_addr(uint8_t * tx_addr)
{
	/* the tx_addr array has the LSB first and the MSB last, assumed to be 4 bytes long */
	uint8_t command = 0x10;
	SPI_write_byte(command);
	SPI_write_byte(tx_addr++);
	SPI_write_byte(tx_addr++);
	SPI_write_byte(tx_addr++);
	SPI_write_byte(tx_addr);
}

uint8_t nrf_read_fifo_status()
{
	return nrf_read_register(0x17);
}

void nrf_flush_tx_fifo()
{
	SPI_write_byte(0xE1);
}

void nrf_flush_rx_fifo()
{
	SPI_write_byte(0xE2);
}

__attribute((always_inline))
void inline nrf_chip_enable()
{
	/* TODO implement function */
	/* raise the CE gpio to logical 1 */
}

__attribute((always_inline))
void inline nrf_chip_disable()
{
	/* TODO implement function */
	/* lower the CE gpio to logical 0 */
}

__attribute((always_inline))
void inline nrf_transmit_enable()
{
	uint8_t config_pwr_down = nrf_read_register(0x00) & 0xED;
	nrf_write_config(config_pwr_down);
}

__attribute((always_inline))
void inline nrf_transmit_disable()
{
	uint8_t config_pwr_up = nrf_read_register(0x00) | 0x02;
	nrf_write_config(config_pwr_up);
}
