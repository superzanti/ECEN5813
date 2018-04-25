/**
 * @file debug.c
 * @brief implements the prototypes in debug.h
 *
 * implements a printing function to hexdump from a location in memory
 * the function should print in a manner similar to a common unix hexdump
 *
 * @author Seth Miers and Jake Cazden
 * @date February 11, 2018
 *
 */

#include"debug.h"

/*included due to usue of PRINTF macro*/
#include"platform.h"

/*
 * @brief function to print the bytes in memory starting at an address
 * 
 * This function takes in a pointer to the start of some
 * memory region and starts pringing out hex bytes
 * for the number of bytes specified.
 *
 * @param start a pointer to the start of the memory region
 * @param length the number of bytes to read back
 * @return void don't return anything
 */
void print_array(void * start, uint32_t length)
{
    #ifdef VERBOSE
    uint32_t i=0;
    uint8_t* temp=(uint8_t*)start;
    for(i=0;i<length;i++)
    {
        if((i)%16==0)/*every 16 bytes (zero inclusive), print the address*/
        {
            PRINTF("%p ",(void*)temp);
        }
        PRINTF("%02X ",*(temp++));
        if((i+1)%16==0)/*after every 16 bytes, print a newline.*/
        {
            PRINTF("\n");/*eg, addr 0-15 \n addr 16-31*/
        }
    }
    #endif
    return;
}
