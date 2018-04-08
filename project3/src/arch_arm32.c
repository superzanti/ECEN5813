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


void InitSysTick()
{
 SysTick_Base_Ptr->CSR &= ~(__SYSTICK_ENABLE_MASK); /* disable counting */
 SysTick_Base_Ptr->RVR = 0xFFFFFF; /* max value */
 SysTick_Base_Ptr->CSR = __SYSTICK_CLKSOURCE_MASK;
 SysTick_Base_Ptr->CSR |= __SYSTICK_ENABLE_MASK; /* enable counting */
/* | __SYSTICK_TICKINT_MASK; * enable the interrupt handler */
}

void SysTick_Handler()
{
}


__attribute__((always_inline))
uint32_t inline ARM32_AIRCR_get_endianness_setting()
{
    return ( (__AIRCR & __AIRCR_ENDIANNESS_MASK) >> __AIRCR_ENDIANNESS_OFFSET ); 
}

__attribute__((always_inline))
uint32_t inline ARM32_CCR_get_stack_alignment()
{
    volatile uint32_t *CCR = (uint32_t *)__CCR; /* volatile for instant access */
    return ((*CCR & __CCR_STK_ALIGNMENT_MASK) >> __CCR_STK_ALIGNMENT_OFFSET);
}

__attribute__((always_inline))
uint32_t inline ARM32_CPUID_get_part_number()
{
    volatile uint32_t *CPUIDReg = (uint32_t *)__CPUID; /* volatile for instant access */
    return ((*CPUIDReg & __CPUID_PART_NO_MASK) >> __CPUID_PART_NO_OFFSET);
}

__attribute__((always_inline))
void inline ARM32_CCR_enable_divide_by_zero_trap()
{
    volatile uint32_t *CCR = (uint32_t *)__CCR; /* volatile for instant access */
    *CCR |= (1 << __CCR_DIVIDE_BY_ZERO_TRAP_OFFSET);
    return;
}

__attribute__((always_inline))
void inline ARM32_CCR_enable_unaligned_access_trap()
{
    volatile uint32_t *CCR = (uint32_t *)__CCR; /* volatile for instant access */
    *CCR |= (1 << __CCR_UNALIGNED_ACCESS_TRAP_OFFSET);
    return;
}

void inline ARM32_create_unaligned_access_trap()
{
    uint8_t unaligned_ptr;
	#pragma GCC diagnostic ignored "-Wunused-variable"
    uint32_t *ptr = (uint32_t *)(&unaligned_ptr);
    /* this should never get here */
    while(1);
}

void inline ARM32_create_divide_by_zero_trap()
{
	#pragma GCC diagnostic ignored "-Wdiv-by-zero"
	#pragma GCC diagnostic ignored "-Wunused-variable"
    uint8_t trap = (1/0);
    /* this should never get here */
    while(1);
}

