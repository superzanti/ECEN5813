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
#ifdef CMOCKA
    #include "memory.h"
    #include "conversion.h"
    #include "data.h"
    #include "circbuf.h"
#endif
#ifdef DEBUG
#include<stdio.h>
#endif

#ifdef PROJECT2
/* static to retain in any scope, const so that the compiler will complain if we touch this from this file */
CB_t* recieve_buffer;
CB_t* transmit_buffer;
#endif
int main(void)
{
    #ifdef PROJECT1
        project1();
    #endif
    #ifdef PROJECT2
	recieve_buffer  = (CB_t*) malloc(sizeof(CB_t));
	transmit_buffer  = (CB_t*) malloc(sizeof(CB_t));
        project2();
    #endif
    #ifdef CMOCKA
        /* TODO create these functions in the header files  */
        const UnitTest tests[] = 
        {
            unit_test(memory_test),
            unit_test(conversion_test),
            unit_test(data_test),
            unit_test(circbuf_test),
        };
        return run_tests(tests);
    #endif
    return 0;
}
