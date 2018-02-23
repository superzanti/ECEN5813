/**
 * @file uart.h
 * @brief function definitions for UART communication and interrupt system
 *
 * This file contains the headers necessary to utilize GPIO pins to
 * realize a UART communication system, this also contains the funcctions
 * to be called from an interrupt vector to process incoming data
 *
 * @author Seth Miers and Jake Cazden
 * @date March 04, 2018
 *
 */

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h> /* for uint8_t */

/* TODO should all size_t's be ints? */
#include <stddef.h> /* for size_t */

/* what the circular buffer should initialize to */
#define BUFFER_LENGTH (128)

typedef enum {
    SUCCESS
} UART_e;

/* @brief configures the specified uart with specified settings
 *
 * @return the status of the function defined by the enum UART_e
 */
UART_e UART_configure();

/* @brief send a single character over the uart
 *
 * @param[in] uint8_t a pointer to a single character to send
 * @return the status of the function defined by the enum UART_e
 */
UART_e UART_send(uint8_t *data);

/* @brief send n characters over the uart
 *
 * @param[in] uint8_t a poitner to an array of characters to send
 * @param[in] size_t the number of bytes to send (treated as int)
 * @return the status of the function defined by the enum UART_e
 */
UART_e UART_send_n(uint8_t *data, size_t num_bytes);

/* @brief recieve a character from the uart
 *
 * @param[out] uint8_t a poitner to the location where the character should be stored
 * @return the status of the function defined by the enum UART_e
 */
UART_e UART_recieve(uint8_t *data);

/* @brief recieve n characters from the uart
 *
 * @param[out] uint8_t a poitner to the start of wehre characters shoudl be stored
 * @return the status of the function defined by the enum UART_e
 */
UART_e UART_recieve_n(uint8_t *data, size_t num_bytes);

/* @brief the itnerrupt request handler for the UART
 */
void UART_IRQHandler();

#endif /*__UART_H__*/
