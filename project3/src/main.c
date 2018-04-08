/**
 * @file main.c
 * @brief calls test functionality for project 1 on multiple platorms
 *
 * this main file suports calling test functions on the project 1
 * function implementations. This will be expanded later to include
 * more functionality
 *
 * @author Seth Miers and Jake Cazden
 * @date February 11, 2017
 *
 */
#include <stdlib.h>
#ifdef PROJECT1
    #include "project1.h"
    #include "circbuf.h"
#endif
#ifdef PROJECT2
    #include "project2.h"
    #include "circbuf.h"
#endif
#ifdef PROJECT3
    #include "project3.h"
    #include "circbuf.h"
#endif
#ifdef CMOCKA
    #include "unittest.h"
#endif
#ifdef DEBUG
#include <stdio.h>
#endif
/*
#ifdef KL25Z
#define CLOCK_SETUP (0)
#endif
*/

#if defined(PROJECT2) || defined(PROJECT3)
/* static to retain in any scope, const so that the compiler will complain if we touch this from this file */
CB_t* recieve_buffer;
CB_t* transmit_buffer;
uint8_t dma0_done=0;
volatile uint32_t DMA_end_value = 0;
#endif
#ifdef KL25Z
volatile uint8_t dma_first_setup = 0;
#endif
volatile uint32_t nooperation=0;
int main(void)
{
    #ifdef CMOCKA
		return unittest();
    #endif

    #ifdef PROJECT1
        project1();
    #endif

    #ifdef PROJECT2
        recieve_buffer  = (CB_t*) malloc(sizeof(CB_t));
        transmit_buffer  = (CB_t*) malloc(sizeof(CB_t));
        project2();
    #endif

	#ifdef PROJECT3
        recieve_buffer  = (CB_t*) malloc(sizeof(CB_t));
        transmit_buffer  = (CB_t*) malloc(sizeof(CB_t));
        project3();
	#endif

    return 0;
}
