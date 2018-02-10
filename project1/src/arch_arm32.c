/**
 * @file arch_arm32.c
 * @brief implementation of arch_arm32.h
 *
 * contains the functional implementation of endianness ARM lookup
 *
 * @author Seth Miers and Jake Cazden
 * @date February 11, 2017
 *
 */
#include "arch_arm32.h"

/* TODO fix warning, may not always be inlinable */
__attribute__((always_inline))
uint32_t ARM32_AIRCR_get_endianness_setting()
{
    return ( (__AIRCR & __AIRCR_ENDIANNESS_MASK) >> __AIRCR_ENDIANNESS_OFFSET ); 
}

