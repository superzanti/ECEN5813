#include "arch_arm32.h"

__attribute__((always_inline))
uint32_t ARM32_AIRCR_get_endianness_setting()
{
    return ( (__AIRCR & __AIRCR_ENDIANNESS_MASK) >> __AIRCR_ENDIANNESS_OFFSET ); 
}

