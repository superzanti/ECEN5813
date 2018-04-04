/*
 * @file project3.c
 * @brief implements project3.h
 *
 *	Project 3 functionality
 *
 *  @author Seth Miers and Jake Cazden
 *  @date March 15, 2018
 */

#ifdef KL25Z
	#include "nordic.h"
	#include "spi.h"
	#include "port.h"
	#include "uart.h"
	#include "circbuf.h"
	#include "conversion.h"
#endif

extern CB_t* recieve_buffer;
extern CB_t* transmit_buffer;

void project3()
{
	GPIO_Configure();
	SPI_init();
    UART_configure();
    uint8_t my_string[] = "Starting project 3...";
    uint8_t return_string[] = "\r\n";
    uint8_t hex_string[] = "Status Reg is: 0x";
    UART_send_n(my_string, sizeof(my_string));
    UART_send_n(return_string, sizeof(return_string));
	uint16_t i = 0x00;
	uint8_t j = 0x00;
	uint8_t statreg = 0x00;
	uint8_t printsize = 0x00;
	for(j=0;j<0xFFFF;j++) /* acts like a counting infinite while loop */
	{
		nrf_write_register(0x05, j);
		statreg = nrf_read_register(0x05);
		printsize = my_itoa((int32_t)statreg, my_string, 16);
		UART_send_n(hex_string, sizeof(hex_string));
		UART_send_n(my_string, printsize);
		UART_send_n(return_string, sizeof(return_string));
		for(i=0;i<0xFFFF;i++);
	}
}
