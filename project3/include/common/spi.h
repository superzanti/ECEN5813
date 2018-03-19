/*
 * @file spi.h
 * @brief
 *
 *
 *
 *  @author Seth Miers and Jake Cazden
 *  @date March 15, 2018
 */

#ifndef __SPI_H_
#define __SPI_H_

/**
 * @brief initializes the spi controller
 *
 * sets up clocks and gpio (using port.h) so that
 * the spi pins are configured properly
 *
 * @param none
 * @return void returns nothing
 */
void SPI_init();

/**
 * @brief reads a single byte from the SPI bus
 *
 * TODO ...
 *
 * @param uint8_t* the byte to read to
 * @return void returns nothing
 */
void SPI_read_byte(uint8_t * byte);

/**
 * @brief writes a single byte to the SPI bus
 *
 * TODO ...
 *
 * @param uint8_t the byte to write
 * @return void returns nothing
 */
void SPI_write_byte(uint8_t byte);

/**
 * @brief send numerous spi bytes
 *
 * repeated calls to SPI_write_byte
 * until all bytes are written
 *
 * @param uint8_t * p a poitner to the byte array to write
 * @param size_t length the number of bytes to send
 * @return void returns nothing
 */
void SPI_send_packet(uint8_t * p, size_t length);

/**
 * @brief flush all data from the bus
 *
 * block the interface until the transmit
 * buffer is empty and has completed transmission
 *
 * @param none
 * @return void returns nothing
 */
void SPI_flush();

#endif /* __SPI_H_ */
