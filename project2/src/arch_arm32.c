/**
 * @file arch_arm32.c
 * @brief implementation of arch_arm32.h
 *
 * contains the functional implementation of endianness ARM lookup
 *
 * @author Seth Miers and Jake Cazden
 * @date February 11, 2018
 *
 */
#include "arch_arm32.h"

__attribute__((always_inline))
uint32_t ARM32_AIRCR_get_endianness_setting()
{
    return ( (__AIRCR & __AIRCR_ENDIANNESS_MASK) >> __AIRCR_ENDIANNESS_OFFSET ); 
}

__attribute__((always_inline))
uint32_t ARM32_CCR_get_stack_alignment()
{
    uint32_t stack_alignment;
    /* TODO implement function */
    return stack_alignment;
}

__attribute__((always_inline))
uint32_t ARM32_CPUID_get_part_number()
{
    uint32_t cpuid;
    /* TODO implement function */
    return cpuid;
}

__attribute__((always_inline))
void ARM32_CCR_enable_divide_by_zero_trap()
{
    /* TODO implement function */
    return;
}

__attribute__((always_inline))
void ARM32_CCR_enable_unaligned_access_trap()
{
    /* TODO implement function */
    return;
}

void ARM32_create_unaligned_access_trap(){
    /* TODO implement function */
    while(1);
}

void ARM32_create_divide_by_zero_trap();
{
    /* TODO implement function */
    while(1);
}

#endif /* __ARCH_ARM32_H__*/

