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
#include<stdlib.h>
#ifdef PROJECT1
    #include "project1.h"
    #include "circbuf.h"
#endif
#ifdef PROJECT2
    #include "project2.h"
    #include "circbuf.h"
#endif
#ifdef PROJECT2
    #include "project3.h"
#endif
#ifdef CMOCKA
    #include "unittest.h"
#endif
#ifdef DEBUG
#include<stdio.h>
#endif
#ifdef KL25Z
#define CLOCK_SETUP (0)
#endif

#ifdef PROJECT2
/* static to retain in any scope, const so that the compiler will complain if we touch this from this file */
CB_t* recieve_buffer;
CB_t* transmit_buffer;
#endif
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
        project3();
	#endif

    return 0;
}
