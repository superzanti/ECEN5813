/*
 * @file project3.h
 * @brief
 *
 *
 *
 *  @author Seth Miers and Jake Cazden
 *  @date March 15, 2018
 */

#ifndef __PROJECT3_H_
#define __PROJECT3_H_

#include "spi.h"

/*
 *@brief tests SPI functionality
 *
 * @param none
 * @return void returns nothing
 *
 */
void project3();

/*
 *@brief profiles the memmove and memset functions
 *
 * outputs results to uart
 *
 * @param none
 * @return void returns nothing
 *
 */
void profiler();

/*
 *@brief tests a few outputs on the SPI
 *
 * outputs results to uart
 *
 * @param none
 * @return void returns nothing
 *
 */
void spi_setup_and_test();


#endif /* __PROJECT3_H_ */
