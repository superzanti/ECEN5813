/**
 * @file arch_arm32.h
 * @brief defines important arm macros and provides hardware interface functions
 *
 * this file contains an endianness lookup function for ARM, as well
 * as relevent memory locations in ARM to allow this
 *
 * @author Seth Miers and Jake Cazden
 * @date February 11, 2017
 *
 */
#ifndef __ARCH_ARM32_H__
#define __ARCH_ARM32_H__

/* Type definitions needed for function prototypes */
#include <stdint.h>

#define __SCB_ADDRESS (0xE0000000)
#define __AIRCR_ADDRESS_OFFSET (0xED0C)
#define __AIRCR (__SCB_ADDRESS & __AIRCR_ADDRESS_OFFSET)
#define __AIRCR_ENDIANNESS_OFFSET (15)
#define __AIRCR_ENDIANNESS_MASK (0x0080)

/*
 * @brief function to get the endianness of a processor
 * 
 * This function uses the defines in this file
 * to direcly dereference the processors memory
 * in order to figure out the endianess of a processor
 *
 * TODO change uint32_t to a smaller usable type
 *
 * @return uint32_t the endianness of the processor
 *          0 for little endian
 *          1 for big endian
 */
uint32_t ARM32_AIRCR_get_endianness_setting();

#endif /* __ARCH_ARM32_H__*/
