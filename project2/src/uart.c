/**
 * @file uart.c
 * @brief implementation of uart.h
 *
 * this file implements uart.h, implementing interrupt functions for the
 * UART communication system
 *
 * @author Seth Miers and Jake Cazden
 * @date March 04, 2018
 *
 */
#include "uart.h"

#include "circbuf.h" /* so we can use the circular buffer in our uart */

#ifdef KL25Z
#include "MKL25Z4.h"
#endif

/* static to retain in any scope, const so that the compiler will complain if we touch this from this file */
static CB_t *const recieve_buffer;
static CB_t *const transmit_buffer;

UART_e UART_configure()
{

    /* set PTE0/PTE20/PTA2/PTA14/PTB17/PTD7 to UART1_TX */
    /* set PTE1/PTE21/PTA1/PTA15/PTB16/PTD6 to UART1_RX */

    /* initialize the buffers */
    CB_init(recieve_buffer, BUFFER_LENGTH);
    CB_init(transmit_buffer, BUFFER_LENGTH);
}
