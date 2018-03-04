/**
 * @file arch_arm32.h
 * @brief defines important arm macros and provides hardware interface functions
 *
 * this file contains an endianness lookup function for ARM, as well
 * as relevent memory locations in ARM to allow this
 *
 * @author Seth Miers and Jake Cazden
 * @date February 11, 2018
 *
 */
#ifndef __ARCH_ARM32_H__
#define __ARCH_ARM32_H__

/* Type definitions needed for function prototypes */
#include <stdint.h>

#define __SCB_ADDRESS (0xE000E000)

#define __AIRCR_ADDRESS_OFFSET (0xD0C)
#define __AIRCR (__SCB_ADDRESS & __AIRCR_ADDRESS_OFFSET)
#define __AIRCR_ENDIANNESS_OFFSET (15)
#define __AIRCR_ENDIANNESS_MASK (0x8000)

#define __CPUID_ADDRESS_OFFSET (0xD00)
#define __CPUID (__SCB_ADDRESS & __CPUID_ADDRESS_OFFSET)
#define __CPUID_PART_NO_OFFSET (4)
#define __CPUID_PART_NO_MASK (0xFFF0)

#define __CCR_ADDRESS_OFFSET (0xD14)
#define __CCR (__SCB_ADDRESS & __CCR_ADDRESS_OFFSET)
#define __CCR_STK_ALIGNMENT_OFFSET (9)
#define __CCR_STK_ALIGNMENT_MASK (0x200)
#define __CCR_UNALIGNED_ACCESS_TRAP_OFFSET (3)
#define __CCR_UNALIGNED_ACCESS_TRAP_MASK (0x8)
#define __CCR_DIVIDE_BY_ZERO_TRAP_OFFSET (4)
#define __CCR_DIVIDE_BY_ZERO_TRAP_MASK (0x10)

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
uint32_t inline ARM32_AIRCR_get_endianness_setting();

/*
 * @brief function to return the current stack alignment
 * 
 * this funciton uses a direct memory dereference of the CCR register
 * in order to calculate the stack alignment value
 *
 * TODO change uint32_t to a smaller usable type
 *
 * @return uint32_t the stack alignment value
 *              0 for 4-byte aligned
 *              1 for 8 byte aligned
 */
uint32_t inline ARM32_CCR_get_stack_alignment();

/*
 * @brief function to return the CPUID (part number)
 * 
 * This function uses direct memory dereferencing of the
 * CPUID register to return the contents.
 *
 * @return uint32_t the cpuid
 */
uint32_t inline ARM32_CPUID_get_part_number();

/*
 * @brief function to write to enable to devide by zero trap
 * 
 * this function uses direct memory dereferencing of the CCR
 * register to enable the divide by zero trap
 *
 * @return void
 */
void inline ARM32_CCR_enable_divide_by_zero_trap();


/*
 * @brief function to write to unaligned access trap
 * 
 * this function uses direct memory dereferencing of the CCR
 * register to enable the unaligned access trap
 *
 * @return void
 */
void inline ARM32_CCR_enable_unaligned_access_trap();

/*
 * @brief create a trap that gets triggerd when an unaligned address is accessed
 * 
 * this function never returns and uses a usage fault exception.
 *
 * @return void
 */
void inline ARM32_create_unaligned_access_trap();


/*
 * @brief performes a divide by zero in order to trap the microcontroller
 * 
 * this function never returns and uses a usage fault exception.
 *
 * @return void
 */
void inline ARM32_create_divide_by_zero_trap();

#endif /* __ARCH_ARM32_H__*/
