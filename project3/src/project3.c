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
	#include "spi.h"
#endif

void project3()
{
	SPI_init();
	while(1)
		SPI_write_byte((uint8_t)0xAA);
}
