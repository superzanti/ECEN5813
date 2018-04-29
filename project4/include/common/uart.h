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

#include <stddef.h> /* for size_t */
/*#ifdef KL25Z
#include"startup_MKL25Z4.S"
#endif
*/
/* what the circular buffer should initialize to *//*doubled this to fit tx message -JC*/
#define BUFFER_LENGTH                   (256)

#define UART0_C4_OSR_SAMPLERATE			(0x0f)
#define BAUD_CLOCK						(CPU_INT_FAST_CLK_HZ)
#define BAUD_CALCULATION(x)             (BAUD_CLOCK/((UART0_C4_OSR_SAMPLERATE+1)*(x)))
#define BAUD_RATE						(9600)
#define CALCULATED_BAUD_MASK            (BAUD_CALCULATION(BAUD_RATE))
#define CLEAR_PCR_ISF                   (1)
#define DISABLE_PCR_IRQC                (0x00)
#define PCR_MUX_ALT2                    (2)

#define SIM_SOPT2_UART0SRC_CLEAR        (3)
#define SIM_SOPT2_UART0SRC_PLLFLLSRC    (1)
#define SIM_SOPT2_UART0SRC_MCGIRCLK     (3)
#define SIM_SOPT2_PLLFLLSEL_CLEAR       (1)
#define SIM_SOPT2_PLLFLLSEL_FLLSRC      (0)

#define MCG_C1_IRCLKEN_ENABLED			(1)
#define MCG_C2_IRCS_FAST				(1)
#define MCG_C2_FCRDIV_CLEAR				(7)
#define MCG_C2_FCRDIV_NODIVISION		(0)

#define SIM_SOPT5_UART0RXSRC_CLEAR      (1)
#define SIM_SOPT5_UART0RXSRC_RXPIN      (0)
#define SIM_SOPT5_UART0TXSRC_CLEAR      (3)
#define SIM_SOPT5_UART0TXSRC_TXPIN      (0)

#define UART0_BDH_LBKDIE_DISABLE        (0)
#define UART0_BDH_RXEDGIE_DISABLE       (0)
#define UART0_BDH_SBNS_SINGLESTOPBIT    (0)

#define SBR_HIGHMASK                    (0x1F00u)
#define SBR_LOWMASK                     (0x00FFu)

#define UART0_C1_LOOPS_NORMALOPERATION  (0)
#define UART0_C1_DOZEEN_ENABLED         (0)
#define UART0_C1_RSRC_DEFAULT           (0)
#define UART0_C1_M_8BIT                 (0)
#define UART0_C1_WAKE_DEFAULT           (0)
#define UART0_C1_ILT_AFTERSTOP          (1)
#define UART0_C1_PE_NOPARITY            (0)
#define UART0_C1_PT_DEFAULTPARTIY       (0)

#define UART0_C2_TIE_DISABLED           (0)
#define UART0_C2_TIE_ENABLED            (1)
#define UART0_C2_TCIE_DISABLED          (0)
#define UART0_C2_TCIE_ENABLED           (1)
#define UART0_C2_RIE_ENABLED            (1)
#define UART0_C2_TLIE_DISABLED          (0)
#define UART0_C2_TE_DISABLED            (0)
#define UART0_C2_TE_ENABLED             (1)
#define UART0_C2_RE_DISABLED            (0)
#define UART0_C2_RE_ENABLED             (1)
#define UART0_C2_RWU_NOWAKEUP           (0)
#define UART0_C2_SBK_NOBREAK            (0)

#define UART0_S1_TDRE_FULL              (0)
#define UART0_S1_TDRE_EMPTY             (1)
#define UART0_S1_RDRF_EMPTY             (0)
#define UART0_S1_RDRF_FULL              (1)

#define UART0_S1_TC_ACTIVE              (0)
#define UART0_S1_TC_IDLE                (1)

typedef enum {
    UART_FAILURE,
    UART_SUCCESS
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
 */ /*TODO does this need an extern or static keyword?*/
void UART0_IRQHandler();

/* @brief enable transmit and interrupt for buffered interupt based transmission
 *
 * @param none
 * @return the status of the function defined by the enum UART_e
 */
UART_e UART_start_buffered_transmission();
#endif /*__UART_H__*/
