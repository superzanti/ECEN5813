/*
 * @file nordic.h
 * @brief
 *
 * Abstraction layer for the nordic NRF chip
 *
 * it's assumed that the NRF is connected to the KL25Z in the following manner
 *       NRF     |     KL25z
 *---------------|---------------
 *        GND    ->    GND
 *        VCC    ->    3.3V
 *        CSN    ->    PTD0
 *        CE     ->    PTD5
 *        SCK    ->    PTD1
 *        MOSI   ->    PTD2
 *        MISO   ->    PTD3
 *        IRQ    ->    NC
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

/* RF SETUP REGISTER
 * The RF setup register on the NRF device is used
 * for setting up how the device outputs.
 * The output power, the low noise amplifier, the data rate,
 * and the PLL are all configured here.
 * For more information see the datasheet by searching for:
 * nRF24L01_Product_Specification_v2_0.pdf
 */

/* CONFIG REGISTER
 * The configuration register on the NRF device is used
 * for setting up the device, enabling and disabling it,
 * enabling or disabling TX, enabling or disabling RX, then
 * powering up or powering down the device. For more information
 * see the NRF datasheet by searching for:
 * nRF24L01_Product_Specification_v2_0.pdf
 */

/* RF CHANNEL REGISTER
 * The RF Channel register is used for setting up the transmit
 * channel. This channel is used so that multiple devices all
 * transmitting over the same frequency can still communicate properly.
 * For more information see the NRF datasheet at:
 * nRF24L01_Product_Specification_v2_0.pdf
 */

/* STATUS REGISTER
 * The Status register on the NRF device is to show the current
 * status of the device. The status includes things such as if the data
 * has been properly transmitted, how many times data should attempt
 * to transmit, if there is anything in the RX buffer, if a fifo
 * should be used for the RX buffer, and if the TX buffer is full.
 * For more information see the NRF datasheet by searching for:
 * nRF24L01_Product_Specification_v2_0.pdf
 */

/* FIFO STATUS REGISTER
 * The FIFO status register on the NRF device shows
 * detailed information on the status of the RX and TX FIFO.
 * This register will represent if the TX has had any reuse,
 * and if the RX and TX FIFOs are full or empty. For more
 * information see the NRF datasheet by searching for:
 * nRF24L01_Product_Specification_v2_0.pdf
 */

#include <stdint.h>

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
 * The Status register on the NRF device is to show the current
 * status of the device. The status includes things such as if the data
 * has been properly transmitted, how many times data should attempt
 * to transmit, if there is anything in the RX buffer, if a fifo
 * should be used for the RX buffer, and if the TX buffer is full.
 * For more information see the NRF datasheet by searching for:
 * nRF24L01_Product_Specification_v2_0.pdf
 *
 * @param none
 * @return uint8_t the value of the reigster
 */
uint8_t nrf_read_status();

/**
 * @brief write to the nrf config register
 *
 * The configuration register on the NRF device is used
 * for setting up the device, enabling and disabling it,
 * enabling or disabling TX, enabling or disabling RX, then
 * powering up or powering down the device. For more information
 * see the NRF datasheet by searching for:
 * nRF24L01_Product_Specification_v2_0.pdf
 *
 * @param uint8_t config the config to write
 * @return void
 */
void nrf_write_config(uint8_t config);

/**
 * @brief read the rf_setup register
 *
 * The RF Channel register is used for setting up the transmit
 * channel. This channel is used so that multiple devices all
 * transmitting over the same frequency can still communicate properly.
 * For more information see the NRF datasheet at:
 * nRF24L01_Product_Specification_v2_0.pdf
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
 * The FIFO status register on the NRF device shows
 * detailed information on the status of the RX and TX FIFO.
 * This register will represent if the TX has had any reuse,
 * and if the RX and TX FIFOs are full or empty. For more
 * information see the NRF datasheet by searching for:
 * nRF24L01_Product_Specification_v2_0.pdf
 *
 * @param none
 * @return uint8_t the value of the reigster
 */
uint8_t nrf_read_fifo_status();

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
extern void inline nrf_chip_enable();

/**
 * @brief disables the nrf chip
 *
 * @param none
 * @return void
 */
extern void inline nrf_chip_disable();

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
