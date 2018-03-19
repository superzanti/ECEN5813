/*
 * @file nordic.h
 * @brief
 *
 * Abstraction layer for the nordic NRF chip
 *
 *  @author Seth Miers and Jake Cazden
 *  @date March 15, 2018
 */

#ifndef __NORDIC_H_
#define __NORDIC_H_

#define NRF_STATUS_REG (0x00)
#define NRF_TXADDR_REG (0x10)
#define NRF_POWER_UP (1)
#define NRF_POWER_DOWN (0)
#define NRF_POWER_UP_MASK (0x02)

/**
 * @brief reads a register and returns the value
 *
 * read a register off of the nrf chip and
 * return the result
 *
 * @param unit8_t readRegister the register to read
 * @return uint8_t the value of the reigster
 */
uint8_t nrf_read_register(uint8_t readRegister);

/**
 * @brief writes to a single register
 *
 * write to a register on the nrf chip
 *
 * @param uint8_t writeRegister the register to write to
 * @param uint8_t value the value to write to the register
 * @return uint8_t the value of the register
 */
void nrf_write_register(uint8_t writeRegister, uint8_t value);

/**
 * @brief reads the status register and returns the result
 *
 * @param none
 * @return uint8_t the value of the reigster
 */
uint8_t nrf_read_status();

/**
 * @brief write to the nrf config register
 *
 * @param uint8_t config the config to write
 * @return void
 */
void nrf_write_config(uint8_t config);

/**
 * @brief read the rf_setup register
 *
 * @param none
 * @return uint8_t the value of the reigster
 */
uint8_t nrf_read_rf_ch();

/**
 * @brief read the 5 bytes for the TX addr
 *
 * @param out uint8_t * address the tx address to be returned
 * @return void
 */
void nrf_read_tx_addr(uint8_t * address);

/**
 * @brief write the TX addr
 *
 * @param unit8_t * tx_addr the tx address to write
 * @return void
 */
void nrf_write_tx_addr(uint8_t * tx_addr);

/**
 * @brief read the fifo_status register
 *
 * @param none
 * @return uint8_t the value of the reigster
 */
void nrf_read_fifo_status();

/**
 * @brief send the flush tx command
 *
 * @param none
 * @return void
 */
void nrf_flush_tx_fifo();

/**
 * @brief send the flush rx command
 *
 * @param none
 * @return void
 */
void nrf_flush_rx_fifo();

/**
 * @brief enables the nrf chip
 *
 * @param none
 * @return void
 */
void inline nrf_chip_enable();

/**
 * @brief disables the nrf chip
 *
 * @param none
 * @return void
 */
void inline nrf_chip_disable();

/**
 * @brief enables the ability to transmit over RF
 *
 * @param none
 * @return void
 */
void inline nrf_transmit_enable();

/**
 * @brief disables the ability to transmit over RF
 *
 * @param none
 * @return void
 */
void inline nrf_transmit_disable();

#endif /* __NORDIC_H_ */
